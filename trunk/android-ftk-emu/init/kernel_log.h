#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H

void log_write(int level, const char *fmt, ...);

#define ERROR(x...)	  log_write(3, "<3>init: " x)
#define NOTICE(x...)  log_write(5, "<5>init: " x)
#define INFO(x...)	  log_write(6, "<6>init: " x)

#endif/*KERNEL_LOG_H*/

