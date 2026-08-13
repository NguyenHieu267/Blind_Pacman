#include "pm_game_eeprom.h"

#define EEPROM_RANKING_ADDR   0x0010
#define EEPROM_SETTING_ADDR 0x0100

#define PM_DEFAULT_RANKING 999

difficulty_t set_difficulty = DIFF_EASY;
sound_state_t set_sound = SOUND_ON;
uint8_t set_time_limit = 40;
uint32_t top_times[3] = {PM_DEFAULT_RANKING, PM_DEFAULT_RANKING, PM_DEFAULT_RANKING};

static bool is_data_loaded = false;

void pm_eeprom_load_data() {
    if (is_data_loaded) return;     // Load EEPROM once (EEPROM -> RAM)
    eeprom_read(EEPROM_RANKING_ADDR, (uint8_t*)top_times, sizeof(top_times));
    if (top_times[0] == 0 || top_times[0] > PM_DEFAULT_RANKING) { 
        top_times[0]=PM_DEFAULT_RANKING; 
        top_times[1]=PM_DEFAULT_RANKING; 
        top_times[2]=PM_DEFAULT_RANKING; 
    }
    
    uint8_t settings[3];
    eeprom_read(EEPROM_SETTING_ADDR, settings, 3);
    if (settings[0] <= 2) { 
        set_difficulty = (difficulty_t)settings[0];
        set_sound = (sound_state_t)settings[1];
        set_time_limit = (int8_t)settings[2];
        if ( set_time_limit != 0 && 
            (set_time_limit < 10 || set_time_limit > 150) ) 
            set_time_limit = 40;
    }
    is_data_loaded = true;
}

// Save data in settings from RAM to EEPROM (RAM -> EEPROM)
void pm_eeprom_save_settings() {
    uint8_t settings[3] = {
        (uint8_t)set_difficulty,
        (uint8_t)set_sound,
        set_time_limit
    };
    eeprom_write(EEPROM_SETTING_ADDR, settings, 3);
}

// Calculate ranking then loaded to EEPROM (RAM -> EEPROM)
void pm_eeprom_update_ranking(uint32_t time) {
    for (int i = 0; i < 3; i++) {
        if (time < top_times[i]) {
            for (int j = 2; j > i; j--) top_times[j] = top_times[j-1];
            top_times[i] = time;
            eeprom_write(EEPROM_RANKING_ADDR, (uint8_t*)top_times, sizeof(top_times));
            break;
        }
    }
}