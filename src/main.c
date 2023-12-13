/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 * Note: 
 * Tested on nRF Connect SDK Version : 2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

//#include <zephyr/drivers/i2c.h>
//#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>



#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

#define SLEEP_TIME_MS   10*60*1000


#define SW0_NODE	DT_ALIAS(sw0) 
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);


#define LED0_NODE	DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);


#define LED1_NODE DT_ALIAS(led1)

static const struct gpio_dt_spec ledGreen = GPIO_DT_SPEC_GET(LED1_NODE, gpios);


void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
}

static struct gpio_callback button_cb_data;

/* UART */
const struct device *uart= DEVICE_DT_GET(DT_NODELABEL(uart0));
static uint8_t tx_buf[] =   {"nRF Connect SDK Fundamentals Course\n\r"
                             "Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\n\r"};

static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {

	case UART_RX_RDY:
		if((evt->data.rx.len) == 1){
			if(evt->data.rx.buf[evt->data.rx.offset] == '1')
				gpio_pin_toggle_dt(&led);
			else if (evt->data.rx.buf[evt->data.rx.offset] == '2')
				gpio_pin_toggle_dt(&ledGreen);	
		}

	break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev ,rx_buf,sizeof rx_buf,RECEIVE_TIMEOUT);
		break;
		
	default:
		break;
	}
}

void main(void)
{
	int ret;

	printk("Message from PRINTK\n");

	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;


	/*if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		hello_world_label = lv_label_create(hello_world_button);
	} else {*/
		//hello_world_label = lv_label_create(lv_scr_act());
	//}

	/*lv_label_set_text(hello_world_label, "HALO WIKTOR !");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);*/



	if (!device_is_ready(led.port)) {
		return;
	}

	if (!gpio_is_ready_dt(&ledGreen)) {
		return;
	}

	if (!device_is_ready(button.port)) {
		return;
	}

	if (!device_is_ready(uart)){
		printk("UART device not ready\r\n");
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return;
	}

	ret = gpio_pin_configure_dt(&ledGreen, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}



	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE );


    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); 	
	

	gpio_add_callback(button.port, &button_cb_data);

	gpio_pin_set_dt(&ledGreen,0);

	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret) {
		return;
	}

	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
	if (ret) {
		return;
	}
	ret = uart_rx_enable(uart ,rx_buf,sizeof rx_buf,RECEIVE_TIMEOUT);
	if (ret) {
		return;
	} 
	// LOG_INF("LOG Starting the loop...");
	// printk("PRINTK Starting the loop...\n");
	k_msleep(1000*2);
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		printk("Device not ready, aborting oled");
		return;
	}

	display_set_brightness(display_dev, 255);
	display_set_contrast(display_dev, 255);
	hello_world_label = lv_label_create(lv_scr_act());

	lv_label_set_text(hello_world_label, "HALO WIKTOR !");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_task_handler();

	display_blanking_off(display_dev);

	while (1) {

        k_msleep(SLEEP_TIME_MS);
	}
}
