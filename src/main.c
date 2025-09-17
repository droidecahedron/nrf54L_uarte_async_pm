#include <string.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>

#define UART_BUF_SIZE 64

static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart20));
static uint8_t rx_buf[UART_BUF_SIZE];
static struct k_work_delayable pm_uartdown_work;
static struct k_work_delayable pm_uartup_work;
static volatile bool want_suspend = false;
static volatile bool suspended = false;

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type)
    {
    case UART_RX_RDY:
        uart_tx(dev, evt->data.rx.buf + evt->data.rx.offset, evt->data.rx.len, SYS_FOREVER_US);
        break;
    case UART_RX_DISABLED:
        if (want_suspend && !suspended)
        {
            k_work_schedule(&pm_uartdown_work, K_NO_WAIT);
        }
        else
        {
            uart_rx_enable(dev, rx_buf, sizeof(rx_buf), 50);
        }
        break;
    default:
        break;
    }
}

static void pm_uartdown_work_handler(struct k_work *work)
{
    int err;
    err = pm_device_action_run(uart_dev, PM_DEVICE_ACTION_SUSPEND);
    // (Optional: handle err)
    suspended = true;
}

static void pm_uartup_work_handler(struct k_work *work)
{
    int err;
    err = pm_device_action_run(uart_dev, PM_DEVICE_ACTION_RESUME);
    err = uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf), 50);
    suspended = false;
}

int main(void)
{
    int err;

    if (!device_is_ready(uart_dev))
    {
        return -1;
    }

    uart_callback_set(uart_dev, uart_cb, NULL);
    uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf), 50);

    k_work_init_delayable(&pm_uartdown_work, pm_uartdown_work_handler);
    k_work_init_delayable(&pm_uartup_work, pm_uartup_work_handler);

    k_msleep(2000); // 2 seconds of being able to echo

    for (;;)
    {
        if (!want_suspend)
        {
            want_suspend = true;
            err = uart_rx_disable(uart_dev);
        }
        else
        {
            want_suspend = false;
            k_work_schedule(&pm_uartup_work, K_NO_WAIT);
        }
        k_sleep(K_MSEC(2000));
    }

    return 0;
}