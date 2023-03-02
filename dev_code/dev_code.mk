DEV_CODE_SOURCES = \
dev_code/entry/src/entry.c \
dev_code/task/driver_task/src/driver_task.c \
dev_code/task/app_task/src/app_task.c \
dev_code/component/led/src/led.c \
dev_code/component/trace/src/trace.c \
dev_code/component/rtc/src/rtc.c \
dev_code/unit_test/test_rtc/src/test_rtc.c \
dev_code/component/button/src/button.c \
dev_code/component/menu/src/menu.c \
dev_code/unit_test/test_menu/src/test_menu.c \
dev_code/component/stub/display.c \
dev_code/component/stub/blood_pressure.c \
dev_code/unit_test/test_button/src/test_button.c \
dev_code/component/stub/storage.c

DEV_CODE_INCLUDES = \
-Idev_code/entry/inc \
-Idev_code/task/driver_task/inc \
-Idev_code/task/app_task/inc \
-Idev_code/component/led/inc \
-Idev_code/component/trace/inc \
-Idev_code/component/rtc/inc \
-Idev_code/component/common/inc \
-Idev_code/unit_test/test_rtc/inc \
-Idev_code/component/button/inc \
-Idev_code/component/menu/inc \
-Idev_code/unit_test/test_menu/inc \
-Idev_code/component/display/inc \
-Idev_code/component/blood_pressure/inc \
-Idev_code/unit_test/test_button/inc \
-Idev_code/component/storage/inc
