#######################################################################
# Include the common makefiles:
#   - Variables:     Sets up the variables with some default values
include make_utils/common_variables.mk
#######################################################################

# Project Name
PROJECT_NAME = chrome_trace

POST_BUILD_TASKS  = cd $(OUTPUT_DIR) && ./$(OUTPUT_FILE);
POST_BUILD_TASKS += speedscope profiler_results.json;


#######################################################################
# Include the common makefiles:
include make_utils/common_executable.mk
include make_utils/common_warnings.mk
include make_utils/common_var_autofill.mk
include make_utils/common_rules.mk
#######################################################################
link_cmd =