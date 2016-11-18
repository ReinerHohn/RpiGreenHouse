# Assign Projectname to variable used in style sheets
export PROJECTNAME	:= $(TARGET)

REMOTE_SSH_CALL		= sshpass -p $(REMOTE_ROOT_PW) ssh $(REMOTE_ROOT)@$(REMOTE) 
REMOTE_SCP_CALL		= sshpass -p $(REMOTE_ROOT_PW) scp 

EXECUTABLE		:= $(PROJECTNAME)

WORKING_DIR		:= /tmp/utest
HOST_BUILD_DIR		:= $(OBJDIR)

GCOV			:= $(CROSS_COMPILE)gcov

# CUnit Files
CUNIT_TOOLS_PATH	:= $(VCS_ROOT)/svn_ent/buildtools/UTest
CUNIT_DTD_FILE		:= CUnit-Run.dtd
CUNIT_RESULTS_FILE	:= CUnitAutomated-Results.xml

# Utest
UTEST_RESULT_DIR	:= $(OBJDIR)/utest
UTEST_RESULTS_FILE_PATH := $(UTEST_RESULT_DIR)/TestresultList.xml

# Coverage
GCOV_RESULT_DIR		:= $(WORKING_DIR)/gcov

LCOV_OUT_DIR		:= $(UTEST_RESULT_DIR)/cov
LCOV_OUT_FILE_PATH	:= $(LCOV_OUT_DIR)/$(PROJECTNAME).info
LCOV_RESULT_LINK	:= cov\/html\/index.html

# Valgrind
VALGRIND_INSTALL_DIR	:= /media/B/valgrind
VALGRIND_LIB		:= $(VALGRIND_INSTALL_DIR)/usr/local/lib/valgrind
VALGRIND_CMD		:= $(VALGRIND_INSTALL_DIR)/usr/local/bin/valgrind
VALGR_RES		:= valgrindResults-$(PROJECTNAME).xml

# target specific flags for 'check' target
check: export CFLAGS  	+= -fprofile-arcs -ftest-coverage
check: export LDFLAGS 	+= -fprofile-arcs
check: export BUILD_TYPE=Unittest

check:
	# 'mode=unittest' used for old code that still uses this switch
	make all mode=unittest
	make start_utest
	make get_utest_result
	make get_valgrind_result
	make get_gcov_result
	make utest_result
	make lcov

check_clean: clean_lcov clean_valgrind clean_utest

install_valgr:
	echo install valgrind
	$(REMOTE_SSH_CALL)"mkdir -p $(VALGRIND_INSTALL_DIR)"
	$(REMOTE_SCP_CALL) valgrind-3.12.tar.gz $(REMOTE_ROOT)@$(REMOTE):$(WORKING_DIR)
	$(REMOTE_SSH_CALL)"tar -xz -C $(VALGRIND_INSTALL_DIR)" < valgrind-3.12.tar.gz
	$(REMOTE_SSH_CALL)"export VALGRIND_LIB=$(VALGRIND_LIB) && $(VALGRIND_CMD) --version"

# kill processes
kill_procs:
	echo killing processes
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 hgs-observer"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 MAIN_APP"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 macs56"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 MACS56"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 macs56-linux-arm-release"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 MACS56-LINUX-ARM-RELEASE"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 macs56-linux-arm-unittest"
	-$(REMOTE_SSH_CALL)"/usr/bin/killall -q -9 MACS56-LINUX-ARM-UNITTEST"

# create utest dir
create_utest_dir:
	echo create utest workingdir
	-$(REMOTE_SSH_CALL)"rm -rfv $(WORKING_DIR)"
	-$(REMOTE_SSH_CALL)"mkdir -p $(WORKING_DIR)"
	-$(REMOTE_SSH_CALL)"mkdir -p $(GCOV_RESULT_DIR)"

# copy unittest executable to target system
copy_executable:
	echo copy executable to target
	$(REMOTE_SCP_CALL) $(HOST_BUILD_DIR)/$(EXECUTABLE) $(REMOTE_ROOT)@$(REMOTE):$(WORKING_DIR)

# start unittest with out folder /tmp
start_utest: install_valgr kill_procs create_utest_dir copy_executable
	echo start unittest
	$(REMOTE_SSH_CALL)"export GCOV_PREFIX=$(GCOV_RESULT_DIR) && export VALGRIND_LIB=$(VALGRIND_LIB) && $(VALGRIND_CMD) --leak-check=full --track-origins=yes --xml=yes --xml-file=$(WORKING_DIR)/$(VALGR_RES) $(WORKING_DIR)/$(EXECUTABLE) /V=$(WORKING_DIR)"
	$(REMOTE_SSH_CALL)"export GCOV_PREFIX=$(GCOV_RESULT_DIR) && export VALGRIND_LIB=$(VALGRIND_LIB) && $(VALGRIND_CMD) --tool=memcheck --leak-check=full --track-origins=yes --xml=yes --xml-file=$(WORKING_DIR)/$(VALGR_RES) $(WORKING_DIR)/$(EXECUTABLE) /V=$(WORKING_DIR)"

# CUnit Resultdatei zurueck auf den Host kopieren
get_utest_result:
	echo fetch unittest results
	$(REMOTE_SCP_CALL) $(REMOTE_ROOT)@$(REMOTE):$(WORKING_DIR)/$(CUNIT_RESULTS_FILE) .

# Copy valgrind Result file zurueck back ot host
get_valgrind_result:
	echo fetch valgrind results
	$(REMOTE_SCP_CALL) $(REMOTE_ROOT)@$(REMOTE):$(WORKING_DIR)/$(VALGR_RES) $(VALGR_RES)
	sed "s@$(WORKING_DIR)/$(EXECUTABLE)@$(PWD)/$(EXECUTABLE)@g" $(VALGR_RES) > $(VALGR_RES).tmp
	mv $(VALGR_RES).tmp $(VALGR_RES)
# Remove line feed 
	perl -p -i -e 's/\R//g;' $(VALGR_RES)
# Insert dummy with #
	sed "s/<\/tid>/<\/tid><dummy>#<\/dummy>/g" $(VALGR_RES) > $(VALGR_RES).dummy
# remove everything after auxwhat entfernen, that does not contain #
	sed "s/<\/auxwhat>[\<a-zA-Z0-9_-./\> ]*<\/stack>/<\/auxwhat>/g" $(VALGR_RES).dummy > $(VALGR_RES).tmp
	rm $(VALGR_RES).dummy
	mv $(VALGR_RES).tmp $(VALGR_RES)
	cat $(VALGR_RES)

# Copy gcov result file bakc ot host
get_gcov_result:
	echo fetch gcov results
	#TODO result path?
	$(REMOTE_SCP_CALL) -r $(REMOTE_ROOT)@$(REMOTE):$(GCOV_RESULT_DIR)/home /

#### UTEST #################
# Generate unittest result file that contains link to utest result and coverage result
utest_result: $(UTEST_RESULTS_FILE_PATH)
	cp $(CUNIT_TOOLS_PATH)/* $(UTEST_RESULT_DIR)/
	chmod +rw $(UTEST_RESULT_DIR)/$(CUNIT_DTD_FILE)
	sed 's%Thu Jan  1 00:00:00 1970%$(shell echo $(shell date +%d.%m.%y' - '%R))%g' $(PROJECT_ROOT)/$(CUNIT_RESULTS_FILE) > $(UTEST_RESULT_DIR)/$(CUNIT_RESULTS_FILE) 
	sed 's%</UNIT_TESTRESULT_LIST>%<UNIT_TESTRESULT><TESTDATE>$(shell echo $(shell date +%d.%m.%y' - '%R))</TESTDATE><PROJECTNAME>$(PROJECTNAME)</PROJECTNAME><TESTTYPE>$(UTEST_TESTTYPE)</TESTTYPE><TESTRESULTLINK>$(CUNIT_RESULTS_FILE)</TESTRESULTLINK><TESTCOVERAGELINK>$(LCOV_RESULT_LINK)</TESTCOVERAGELINK></UNIT_TESTRESULT></UNIT_TESTRESULT_LIST>%g' $(UTEST_RESULTS_FILE_PATH) > $(UTEST_RESULTS_FILE_PATH).tmp
	mv $(UTEST_RESULTS_FILE_PATH).tmp $(UTEST_RESULTS_FILE_PATH)

$(UTEST_RESULT_DIR):
	mkdir -p $(UTEST_RESULT_DIR)

$(UTEST_RESULTS_FILE_PATH): $(UTEST_RESULT_DIR)
	echo "<?xml-stylesheet type=\"text/xsl\" href=\"TestresultListC.xsl\" ?><UNIT_TESTRESULT_LIST><TITLE>Testslist</TITLE></UNIT_TESTRESULT_LIST>" > $(UTEST_RESULTS_FILE_PATH)

#### COVERAGE ##############
lcov: 
	mkdir -p $(LCOV_OUT_DIR)
	lcov -d . -b . -c -o $(LCOV_OUT_FILE_PATH) --gcov-tool $(GCOV)
	genhtml -o $(LCOV_OUT_DIR)/html $(LCOV_OUT_FILE_PATH) --num-spaces 3 --no-branch-coverage
	#chmod +rw $(LCOV_OUT_DIR)/html/index.html
	#@$(ECHO) $(LCOV_OUT_DIR)/html/index.html
	lcov --list $(LCOV_OUT_FILE_PATH) | grep "Total:" > $(LCOV_OUT_DIR)/$(PROJECTNAME).summary
	#(cd $(VCS_ROOT) &&  python /usr/local/lib/python2.7/dist-packages/lcov_cobertura.py $(LCOV_OUT_FILE_PATH) )

#### CLEAN ################
clean_utest: clean_lcov clean_valgrind
	rm -rf $(UTEST_RESULT_DIR)

clean_valgrind:
	rm -f $(VALGR_RES)

clean_lcov:
	rm -rf $(LCOV_OUT_DIR)

