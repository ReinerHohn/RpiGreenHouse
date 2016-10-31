#!/usr/bin/env python

import sys
import getopt
import pystache
import yaml
import os
import os.path
import uuid

def usage():
    print("Usage: qtcreator_project.py [options] source_dir builddir proj_name target_name target_uuid")
    print("Options:\n\t-h show this help")

def render_template(tpl_path, doc_path, ctx):
    with open(tpl_path) as tpl:
        with open(doc_path, "w") as doc:
            doc.write(pystache.render(tpl.read(), ctx))

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
    if len(args) != 5:
        usage()
        sys.exit(2)

    # load yaml build config
    # cfg = yaml.load(open(args[0]))

    # convert to template compatible structure
    #files = []
    #for file in cfg['sources']:
    #    if os.access(file, os.F_OK):
    #        files.append({'name': os.path.basename(file), 'path': file})
    #cfg['sources'] = files

    # misc files
    #files = []
    #for file in cfg['files']:
    #    if os.access(file, os.F_OK):
    #        files.append({'name': os.path.basename(file), 'path': file})
    #cfg['files'] = files

    cfg = {}
    cfg['env_id'] = uuid.uuid4()
    cfg['proj_conf_id'] = uuid.uuid4()
    cfg['source_dir'] = args[0]
    cfg['build_dir'] = args[1]
    cfg['proj_name'] = args[2]
    cfg['target_name'] = args[3]
    cfg['target_uuid'] = args[4]

    try:
        os.makedirs(cfg['build_dir'])
    except Exception as e:
        pass

    lists_txt_path = os.path.join(cfg['source_dir'], 'CMakeLists.txt.user')

    if os.path.exists(lists_txt_path):
        os.remove(lists_txt_path)

    # create CMakelists.txt.user file
    render_template("CMakeLists.user-tpl", lists_txt_path, cfg)
if __name__ == "__main__":
    main()
