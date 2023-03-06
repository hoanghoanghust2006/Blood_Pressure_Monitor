DEV_CODE_SOURCES = \
dev_code/entry/src/entry.c \
dev_code/task/driver_task/src/driver_task.c \
dev_code/task/app_task/src/app_task.c \
dev_code/component/led/src/led.c \
dev_code/component/trace/src/trace.c \
dev_code/component/glcd/src/glcd.c \
dev_code/component/glcd/src/font_glcd.c \
dev_code/component/common/src/common.c \
dev_code/unit_test/test_button/src/test_button.c \
dev_code/unit_test/test_glcd/src/test_glcd.c \
dev_code/component/rtc/src/rtc.c \
dev_code/unit_test/test_rtc/src/test_rtc.c \
dev_code/component/button/src/button.c

DEV_CODE_INCLUDES = \
-Idev_code/entry/inc \
-Idev_code/task/driver_task/inc \
-Idev_code/task/app_task/inc \
-Idev_code/component/led/inc \
-Idev_code/component/trace/inc \
-Idev_code/component/glcd/inc \
-Idev_code/component/common/inc \
-Idev_code/unit_test/test_button/inc \
-Idev_code/unit_test/test_glcd/inc \
-Idev_code/component/rtc/inc \
-Idev_code/unit_test/test_rtc/inc \
-Idev_code/component/button/inc