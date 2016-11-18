VCS_ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))../../)

# Add files for unittest if target 'check' is executed
ifneq ($(filter check,$(MAKECMDGOALS)),)
export SRCS 			+= $(SOURCE_FILES_UTEST)
endif

# Create dependeny list from 'SRCS' variable 
_SRCS := $(shell realpath $(filter-out $(EXCLUDE_SRCS), $(SRCS)))
OBJS := $(addprefix $(OBJDIR)/,$(filter %.o,$(_SRCS:.c=.o))) $(addprefix $(OBJDIR)/,$(filter %.o,$(_SRCS:.cpp=.o)))
DEPS := $(OBJS:.o=.d)

# Directory to install built libraries into. This path is added to to search for libraries to link against.
export LIBS_INSTALL_DIR:=$(VCS_ROOT)/libdir/$(OBJDIR)

CFLAGS_DEBUG 		?= -g -O0 -fno-omit-frame-pointer -D_DEBUG
CXXFLAGS_DEBUG 		?= -g -O0 -fno-omit-frame-pointer -D_DEBUG
CFLAGS_RELEASE 		?= -Os -DNDEBUG
CXXFLAGS_RELEASE 	?= -Os -DNDEBUG
CFLAGS_UNITTEST 	?= -g -O0 -D_DEBUG -DBOS_UNIT_TESTS
CXXFLAGS_UNITTEST 	?= -g -O0 -D_DEBUG -DBOS_UNIT_TESTS

ifeq ($(BUILD_TYPE),)
BUILD_TYPE = Debug
endif

ifeq ($(BUILD_TYPE),Debug)
override CFLAGS += $(CFLAGS_DEBUG)
override CXXFLAGS += $(CXXFLAGS_DEBUG)
endif

ifeq ($(BUILD_TYPE),Release)
override CFLAGS += $(CFLAGS_RELEASE)
override CXXFLAGS += $(CXXFLAGS_RELEASE)
endif

# Set revision
ifeq ($(UTEST_TESTTYPE),Datentest)
REV:=0
else
ifeq ($(UTEST_TESTTYPE),Unittest)
REV:=0
else
REV:=0
# REV:=$(shell svn info $(SOURCE_PREFIX) | grep Revision | sed 's/Revision: //')
endif
endif

ifneq ($(REV),)
  CFLAGS += -DREVISION=$(REV)
endif 

#override CFLAGS += "-L$(LIBS_INSTALL_DIR)"

# Add Unittest flags if target 'check' is executed
ifneq ($(filter check,$(MAKECMDGOALS)),)
override CFLAGS 	+= $(CFLAGS_INCLUDE_PREPEND_UTEST) $(CFLAGS_UNITTEST)
override LDFLAGS 	+= $(LDFLAGS_UTEST)
endif

# Add pkg-config paths
ifneq ($(PKG_CONFIG_SYSROOT_DIR),)
	PKG_CONFIG_ENV += PKG_CONFIG_SYSROOT_DIR=$(PKG_CONFIG_SYSROOT_DIR)
endif
ifneq ($(PKG_CONFIG_DIR),)
	PKG_CONFIG_ENV += PKG_CONFIG_DIR=$(PKG_CONFIG_DIR)
endif
ifneq ($(PKG_CONFIG_LIBDIR),)
	PKG_CONFIG_ENV += PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR)
endif

# Add flags with pkg-config from a list of thirdparty libs (PACKAGES)
ifneq ($(PACKAGES),)
override CFLAGS += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags $(PACKAGES))
override CXXFLAGS += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags $(PACKAGES))
override LDFLAGS += $(shell $(PKG_CONFIG_ENV) pkg-config --libs $(PACKAGES))
override LDLIBS += $(shell $(PKG_CONFIG_ENV) pkg-config --libs $(PACKAGES))
endif

# Add all targets to be built to 'TARGETS' as dependency for 'all' target
ifneq ($(TARGET),)
TARGETS += $(OBJDIR)/$(TARGET)
endif

ifneq ($(LIB_TARGET),)
TARGETS += $(OBJDIR)/$(LIB_TARGET)
endif

ifneq ($(STATIC_LIB_TARGET),)
TARGETS += $(OBJDIR)/$(STATIC_LIB_TARGET)
endif

ifneq ($(strip $(NO_ALL_TARGET)),YES)
all: greeting $(EXTRA_TARGETS) $(TARGETS)
	@echo Done
endif

# Add helpers for tests and external tools like eclipse, sonarcube,...
include $(SOURCE_PREFIX)/projects/buildtools/tester.mk
include $(SOURCE_PREFIX)/projects/buildtools/external_toolgen.mk

-include $(DEPS)

define deps_fixup
	cp -f $(patsubst %.o,%.d,$@) $(patsubst %.o,%.d,$@).tmp
	sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	rm -f $(patsubst %.o,%.d,$@).tmp
endef

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $@
	
$(LIBS_INSTALL_DIR):
	mkdir LIBS_INSTALL_DIR $(LIBS_INSTALL_DIR) oooooooooooooooooooooooooooooooooooooo
	mkdir -p $@

# Build target 
ifneq ($(TARGET),)
#DEPLOY_FILES += $(OBJDIR)/$(TARGET)
$(OBJDIR)/$(TARGET): $(OBJS)
	@echo [LINK] $@
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
endif

# build shared libs targets
ifneq ($(LIB_TARGET),)
DEPLOY_FILES += $(OBJDIR)/$(LIB_TARGET)
override CFLAGS += -fPIC
$(OBJDIR)/$(LIB_TARGET): $(OBJS)
	@echo [LINK] $@
	$(CC) -shared -Wl,-soname,$(SONAME) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
endif

# build static libs targets
ifneq ($(STATIC_LIB_TARGET),)
#DEPLOY_FILES += $(OBJDIR)/$(STATIC_LIB_TARGET)
override CFLAGS += -fPIC
$(OBJDIR)/$(STATIC_LIB_TARGET): $(OBJS) $(LIBS_INSTALL_DIR)
	@echo [LINK] $@
	$(AR) rvc $@ $^
	echo install $@ $(LIBS_INSTALL_DIR) oooooooooooooooooooooooooooooooooooo
	install $@ $(LIBS_INSTALL_DIR)
endif

# Build all c-files and create dependency files (.d)
$(OBJDIR)/%.o: %.c
	@echo [CC] $*.c
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -o $@ $<
	$(call deps_fixup)

$(OBJDIR)/%.o: %.cpp
	@echo [CXX] $*.c
	mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -o $@ $<
	$(call deps_fixup)

clean: $(EXTRA_CLEAN_TARGETS)
	@echo Cleaning $(TARGETS)
	@echo Cleaning $(OBJDIR)
	@echo Cleaning $(CLEAN_FILES)
	rm -rf $(OBJDIR)
	rm -rf $(TARGETS)
ifneq ($(CLEAN_FILES),)
	rm -rf $(CLEAN_FILES)
endif
	@echo Done

dist_clean: clean clean_ws
	rm -rf builds

greeting:
	@echo Building $(TARGET) in mode $(BUILD_TYPE)

help:
	@echo supported targets:
	@echo -e "\thelp\t\t\t\tshow this help"
	@echo -e "\tall\t\t\t\tbuild the default target"
	@echo -e "\tclean\t\t\t\tclean build directory"
	@echo -e "\techo\t\t\t\toutput ide relevant variables"
	@echo -e "\tdoxygen\t\t\t\tCreate doxygen docu"
	@echo -e "\tcreate_sonar_cube_helper\tCreate sonarcube file"
	@echo -e "\tdeploy\t\t\t\tdeploy to target"
	@echo -e "\teclipse\t\t\t\tCreate eclipse project"
	@echo -e ""
	@echo -e "\tusage deploy:"
	@echo -e "\t\tmake deploy [REMOTE=<target-ip> REMOTE_ROOT=<username> REMOTE_ROOT_PW=<REMOTE_ROOT_PWord> REMOTE_EXE_PATH=<path> ]"
# print out all usefull command line options
	@echo -e "\tusage eclipse:"
	@echo -e "\t\tmake eclipse [REMOTE=<target-ip> REMOTE_ROOT=<username> REMOTE_ROOT_PW=<REMOTE_ROOT_PWord> REMOTE_EXE_PATH=<path> ]"

define DEPLOY_CMD
	@echo [deploy] $(FILE) to $(REMOTE_ROOT)@$(REMOTE):$(REMOTE_EXE_PATH)
	sshpass -p $(REMOTE_ROOT_PW) scp -o 'StrictHostKeyChecking no' -r $(FILE) $(REMOTE_ROOT)@$(REMOTE):$(REMOTE_EXE_PATH)
endef

deploy: $(EXTRA_TARGETS) $(DEPLOY_FILES)
	$(foreach FILE, $(DEPLOY_FILES), $(DEPLOY_CMD))

deploy_file:
	$(DEPLOY_CMD)

# Create config.yml file that describes project 
$(OBJDIR)/config.yml: $(OBJDIR)
	echo "project: '$(notdir $(TARGETS))-$(BUILD_TYPE)-$(SYSTEM_NAME)-$(SYSTEM_PROCESSOR)'" > $@
	echo >> $@
	echo "project_name: $(TARGET)" >> $@
	echo >> $@
	echo "target: $(TARGETS)" >> $@
	echo >> $@
	echo "objdir: $(OBJDIR)" >> $@
	echo >> $@
	echo "build_loc: $(PROJECT_ROOT)" >> $@
	echo >> $@
	echo "svn_root: $(VCS_ROOT)" >> $@
	echo >> $@
	echo "workspace_dir: $(WORKSPACE_DIR)" >> $@
	echo >> $@
	echo "ip_target: $(REMOTE)" >> $@
	echo >> $@
	echo "remote_user: $(REMOTE_USER)" >> $@
	echo >> $@
	echo "remote_target_path: $(REMOTE_EXE_PATH)$(TARGET)" >> $@
	echo >> $@
	echo "prerun_cmd: $(PRE_RUN_CMD)" >> $@
	echo >> $@
	echo "prog_args: $(REMOTE_ARGS)" >> $@
	echo >> $@
	echo "gdb_exe: $(CROSS_COMPILE)gdb" >> $@
	echo >> $@
	echo "defines:" >> $@
	echo | $(CC) $(CPPFLAGS) $(CFLAGS) -E -dM - | awk '{print "   - name: "$$2; $$1=""; $$2=""; gsub(/^[\t ]+/, "", $$0); gsub(/"/, "\\\"", $$0); print "     value: \""$$0"\""}' >> $@
	$(VCS_ROOT)/projects/buildtools/get_defines.sh $(CPPFLAGS) $(CFLAGS) | awk '{print "   - name: "$$2; $$1=""; $$2=""; gsub(/^[\t ]+/, "", $$0); gsub(/"/, "\\\"", $$0); print "     value: \""$$0"\""}' >> $@
	echo >> $@
	echo "includes:" >> $@
	echo | LANG=C $(shell $(CC) -print-prog-name=cc1) $(CPPFLAGS) $(CFLAGS) -v 2>&1 | awk '/End of search list./{exit}; /^ /{gsub(/^ /, "   - ", $$0); print $$0}' >> $@
	echo >> $@
	echo "c_files:" >> $@
	for F in $(_SRCS); do echo "   - $$F" >> $@; done
	echo "sources:" >> $@
	for F in $(_SRCS); do echo "   - $$F" >> $@; done
	for F in $(_SRCS:.c=.h); do echo "   - $$F" >> $@; done
	for F in $(EXTRA_FILES); do echo "   - $$F" >> $@; done
	echo >> $@
	echo "files:" >> $@
ifdef MISC_FILES
	for F in $(shell realpath $(MISC_FILES)); do echo "   - $$F" >> $@; done
endif
	echo "   - $(PROJECT_ROOT)/Makefile" >> $@

# print out all importand internal variables
.PHONY: echo
echo:
	@$(ECHO) "define variables:"
	@$(ECHO) "  BUILD_TYPE=$(BUILD_TYPE)"
	@$(ECHO) "  CC=$(CC)"
	@$(ECHO) "  CPP=$(CPP)"
	@$(ECHO) "  LD=$(LD)"
	@$(ECHO) "  AS=$(AS)"
	@$(ECHO) "  STRIP=$(STRIP)"
	@$(ECHO) "  CFLAGS=$(CFLAGS)"
	@$(ECHO) "  CXXFLAGS=$(CXXFLAGS)"
	@$(ECHO) "  LDFLAGS=$(LDFLAGS)"
	@$(ECHO) "  LDFLAGS_LIBS=$(LDFLAGS_LIBS)"
	@$(ECHO) "  SRCS=$(SRCS)"
	@$(ECHO) "  SRCS_UTEST=$(SRCS_UTEST)"
	@$(ECHO) "  MAKEFILE_LIST=$(MAKEFILE_LIST)"
	@$(ECHO) "  MAKEFILENAME=$(MAKEFILENAME)"

.PHONY: all greeting echo preprocessor deploy help $(OBJDIR)/config.yml

ifneq ($(V),1)
.SILENT:
endif
