#######################################################################
# Include the common makefiles:
#   - Variables:     Sets up the variables with some default values
include make_utils/common_variables.mk
#######################################################################

# Project Name
PROJECT_NAME = chrome_trace

# Add your souce directories here
SOURCE_DIRS = .

DEFINES += -DPROFILER_FILENAME=profiler.json

# Include paths
INC_DIRS = .

#######################################################################
# Include the common makefiles:
include make_utils/common_executable.mk
include make_utils/common_warnings.mk
include make_utils/common_var_autofill.mk
include make_utils/common_rules.mk
#######################################################################
