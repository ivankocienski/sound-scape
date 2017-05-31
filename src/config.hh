#pragma once

#include <vector>
#include <string>

void config_parse_args(int, char**);
bool config_is_valid();
void config_show_help();

extern bool config_wants_help;
extern bool config_debug;
//extern int config_chunk_time;
//extern int config_margin_time;
//extern int config_fade_in_time;

extern std::vector<std::string> config_data_sources;

