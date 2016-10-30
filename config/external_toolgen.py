#!/usr/bin/env python

import sys
import getopt
import pystache
import yaml
import os
import os.path
import uuid

def usage():
    print("Usage: eclipse_project.py [options] config.yml function-flag")
    print("Options:\n\t-h show this help")

def render_template(tpl_path, doc_path, ctx):
    with open(tpl_path) as tpl:
        with open(doc_path, "w") as doc:
            doc.write(pystache.render(tpl.read(), ctx))

def create_sonar_cube_helper(cfg):
    SOURCES = cfg['c_files']
    INCLUDES = cfg['includes']

    SRC_TEMP = ""
    for src in SOURCES:
	src = src.replace(cfg['svn_root'] + "/", '')
	SRC_TEMP += src + ","

    INC_TEMP = ""
    for inc in INCLUDES:
	inc = inc.replace(cfg['svn_root'] + "/", '')
	INC_TEMP += inc + ","

#TODO if not exitst set empty

    cfg['sonar_sources'] = SRC_TEMP
    cfg['sonar_includes'] = INC_TEMP

    # create '.sonar-project.properties' file
    render_template(os.path.join(cfg['tpl_path'], "sonar-project.properties.tpl"), os.path.join(cfg['svn_root'], '.sonar-project.properties'), cfg) 

def create_doxygen_file(cfg):
    files = ""
    for file in cfg['sources']:
        if os.access(file, os.F_OK):
            files += file + ' '
    cfg['sources'] = files

    includes = ""
    for incl in cfg['includes']:
        if os.access(incl, os.F_OK):
            includes += incl + ' '
    cfg['includes'] = includes
   
    defines = ""
    for define in cfg['defines']:
            defines += '\'' + define['name'] + '=' + define['value'] + '\' '
    cfg['defines'] = defines

    TPL_PATH = cfg['tpl_path']
    OUTPUT = cfg['objdir']
    render_template(os.path.join(TPL_PATH, "Doxygen.Doxyfile.tpl"), os.path.join(OUTPUT, 'Doxygen.Doxyfile'), cfg)  

def create_eclipse_proj(cfg):
    # convert to template compatible structure
    files = []
    for file in cfg['sources']:
        if os.access(file, os.F_OK):
            files.append({'name': os.path.basename(file), 'path': file})
    cfg['sources'] = files

    # misc files
    files = []
    for file in cfg['files']:
        if os.access(file, os.F_OK):
            files.append({'name': os.path.basename(file), 'path': file})
    cfg['files'] = files

    TPL_PATH = cfg['tpl_path']
    OUTPUT = cfg['objdir']

    # If no extra argument is given (like TOOLCHAIN_FILE), set it to not "None"
    if cfg['arguments'] is None:
	cfg['arguments'] = ""

    # Workaround to be able to deploy shared libraries (added build step deploy)
    cfg['build_target'] = "all deploy " + cfg['arguments']

    # create .project file
    render_template(os.path.join(TPL_PATH, "dot-project.tpl"), os.path.join(OUTPUT, '.project'), cfg)
    # create .cproject file
    render_template(os.path.join(TPL_PATH, "dot-cproject.tpl"), os.path.join(OUTPUT, '.cproject'), cfg)
    # create .settings/language.settings.xml file
    try:
        os.makedirs(os.path.join(os.path.dirname(sys.argv[1]), ".settings"))
    except Exception as e:
        pass
    render_template(os.path.join(TPL_PATH, "dot-settings-language.settings.tpl"), os.path.join(OUTPUT, '.settings', 'language.settings.xml'), cfg)


def create_eclipse_sess(cfg):
    WORKSPACE_DIR = cfg['workspace_dir']
    cfg['executable_path'] = os.path.abspath(cfg['target'])
    TARGET_NAME = os.path.basename(cfg['target'])
    cfg['project_name'] = TARGET_NAME

    SUB_NAME = "H." + TARGET_NAME + "_0"
    cfg['node_entry'] = "05-ref.00001=" + SUB_NAME

    ECLIPSE_LAUNCH_PATH = os.path.join(WORKSPACE_DIR, ".metadata/.plugins/org.eclipse.debug.core/.launches/")
    NODE_PROP_SSH_PATH = os.path.join(WORKSPACE_DIR, ".metadata/.plugins/org.eclipse.rse.core/profiles/PRF.localhost_0/")
    NODE_PROP_SSH_PATH2 = os.path.join(WORKSPACE_DIR, ".metadata/.plugins/org.eclipse.rse.core/profiles/PRF.localhost_0/", SUB_NAME + "/")
    CORE_SET_PATHS = os.path.join(WORKSPACE_DIR, ".metadata/.plugins/org.eclipse.core.runtime/.settings/")
    DGB_SET_PATHS = os.path.join(WORKSPACE_DIR, ".metadata/.plugins/org.eclipse.debug.ui/")


    if not os.path.exists(cfg['objdir']):
	os.makedirs(cfg['objdir'])

    if not os.path.exists(ECLIPSE_LAUNCH_PATH):
	os.makedirs(ECLIPSE_LAUNCH_PATH)
 
    if not os.path.exists(NODE_PROP_SSH_PATH):
	os.makedirs(NODE_PROP_SSH_PATH)
      
    if not os.path.exists(NODE_PROP_SSH_PATH2):
	os.makedirs(NODE_PROP_SSH_PATH2)

    if not os.path.exists(CORE_SET_PATHS):
	os.makedirs(CORE_SET_PATHS)

    if not os.path.exists(DGB_SET_PATHS):
	os.makedirs(DGB_SET_PATHS)

    TPL_PATH = cfg['tpl_path']

    # Create debug configuration (if TOOLCHAIN_FILE is given, use remote configuration)
    if 'TOOLCHAIN_FILE' in cfg['arguments']:
	render_template(os.path.join(TPL_PATH, "eclipse-launch-remote.tpl"), os.path.join(ECLIPSE_LAUNCH_PATH, TARGET_NAME + '.launch'), cfg)
    else:    
	render_template(os.path.join(TPL_PATH, "eclipse-launch-local.tpl"), os.path.join(ECLIPSE_LAUNCH_PATH, TARGET_NAME + '.launch'), cfg)

    # Create default entry for debug seesion
    render_template(os.path.join(TPL_PATH, "launchConfigurationHistory.xml.tpl"), os.path.join(DGB_SET_PATHS, "launchConfigurationHistory.xml"), cfg)

    # Add ssh connection to selection
    render_template(os.path.join(TPL_PATH, "ui.prefs.tpl"), os.path.join(CORE_SET_PATHS, "org.eclipse.rse.ui.prefs"), cfg)

    # set remote username
    render_template(os.path.join(TPL_PATH, "core.prefs.tpl"), os.path.join(CORE_SET_PATHS, "org.eclipse.rse.core.prefs"), cfg)
	
    # Add ssh connection node entry
    render_template(os.path.join(TPL_PATH, "node.properties.tpl"), os.path.join(NODE_PROP_SSH_PATH, "node.properties"), cfg)

    # Create ssh connection node
    render_template(os.path.join(TPL_PATH, "ssh-only.node.properties.tpl"), os.path.join(NODE_PROP_SSH_PATH2, "node.properties"), cfg)

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"])

	if len(args) != 2:
           usage()
           sys.exit(2)

	# load yaml build config
        cfg = yaml.load(open(args[0]))

	cfg['tpl_path'] = os.path.join(os.path.dirname(sys.argv[0]), "templates")

    	try:
	    OUTPUT = cfg['objdir']
            os.makedirs(OUTPUT)
    	except Exception as e:
            pass

    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for o in args:
	if o in ("-e", "--eclipse"):
	    create_eclipse_proj(cfg)
	    create_eclipse_sess(cfg)
        elif o in ("-s", "--sonarcube"):
	    create_sonar_cube_helper(cfg)
	elif o in ("-d", "--doxygen"):
	    create_doxygen_file(cfg)
	elif o in ("-h", "--help"):
            usage()
            sys.exit()

if __name__ == "__main__":
    main()
