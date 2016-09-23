#!/usr/bin/env python

import sys
import getopt
import pystache
import yaml
import os
import os.path
import uuid

def usage():
    print("Usage: qtcreator_project.py [options] config.yml objdir builddir toolchainfile")
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
    if len(args) != 4:
        usage()
        sys.exit(2)

    # load yaml build config
    cfg = yaml.load(open(args[0]))

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

    cfg['config_id'] = uuid.uuid4()
    cfg['build_loc'] = args[2]
    cfg['arguments'] = args[3]

    try:
        os.makedirs(args[1])
    except Exception as e:
        pass

    # create CMakelists.txt.user file
    render_template(os.path.join(os.path.dirname(sys.argv[0]), "dot-project.tpl"), os.path.join(args[1], 'CMakelists.txt.user'), cfg)
if __name__ == "__main__":
    main()
