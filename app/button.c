#include "multi_button.h"

#include "adc.h"

#include "user_task.h"
#include "button.h"
#include "oled.h"

#define BTN_ADC_NAME "adc1"

enum BTN_NAME
{
	BTN_UP,
	BTN_DOWN,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_NUM,
};

event_id_t EVE_BUTTON = 1 << 0;

#define BTN_ACTIVE_LEVEL (0)
#define LPRESS_EVE_DELAY (200 / TICKS_INTERVAL)

static adc_device_t *dev;

struct Button* btn[BTN_NUM];
static user_data_t btn_data;

static uint8_t pin_lvl(uint8_t pin);
static void btn_long_press_cb(void* handle);
static void btn_single_click_cb(void* handle);

static void button_initial(task_id_t task_id)
{
	dev = cd_adc_device_find(BTN_ADC_NAME);
	CD_ASSERT(dev);

	btn_data.value = cd_malloc(sizeof(uint8_t));
	CD_ASSERT(btn_data.value);

	btn_data.id = USER_ID_BTN;
	for(uint8_t i = 0; i < BTN_NUM; i++)
	{
		btn[i] = (struct Button*)cd_malloc(sizeof(struct Button));
		CD_ASSERT(btn[i]);
		button_init(btn[i], pin_lvl, BTN_ACTIVE_LEVEL, i);
		button_attach(btn[i], SINGLE_CLICK, btn_single_click_cb);
		button_attach(btn[i], LONG_PRESS_HOLD, btn_long_press_cb);
		button_start(btn[i]);
	}

    cd_timer_startre(task_id, EVE_BUTTON, TICKS_INTERVAL);
}

static cd_uint16_t button_process(task_id_t task_id, event_asb_t events)
{
	(void)task_id;

    if(events & EVE_BUTTON)
    {
		button_ticks();
		cd_task_seteve(TASK_TEST, EVE_TEST);

        return events ^ EVE_PRINTSTR;
    }

    return 0;
}

static cd_task_ops_t button_ops = {button_initial, button_process};

static void button_task(void)
{
	cd_task_create(&button_ops, TASK_BUTTON);
	draw_dino();
}
INIT_APP_EXPORT(button_task);

static void btn_single_click_cb(void* handle)
{
	uint8_t id;
	id = ((struct Button*)handle)->button_id;
	*(uint8_t *)btn_data.value = id;
	LOG_W("BTN %d CLICKED", id);
}

static void btn_long_press_cb(void* handle)
{
	uint8_t id;
	static uint8_t cnt;

	cnt++;
	//delay so that long press hold send event more smooth other than light speed
	if(0 == cnt % LPRESS_EVE_DELAY)
	{
		id = ((struct Button*)handle)->button_id;
		*(uint8_t *)btn_data.value = (id + 1) << 4;
		LOG_W("BTN %d LPRESSED", id);
	}
}

uint8_t pin_lvl(uint8_t pin)
{
	uint8_t pin_index;
	cd_uint32_t val;

	val = cd_adc_device_read(dev);
	if(val != CD_ERROR)
	{
		if(val > 3000){pin_index = 0xFF;}
		else if(val < 3000 && val > 2600){pin_index = BTN_DOWN; }
		else if(val < 2600 && val > 2000){pin_index = BTN_RIGHT; }
		else if(val < 2000 && val > 200){pin_index = BTN_LEFT; }
		else if(val < 200 && val > 0){pin_index = BTN_UP; }
	}
	else
	{
		LOG_E("ADC READ BTN FAILED");
	}

	if(pin_index != pin){return !BTN_ACTIVE_LEVEL;}

	return BTN_ACTIVE_LEVEL;
}