#pragma once

#include <vector>
#include <string>

void config_parse_args(int, char**);
bool config_is_valid();
void config_show_help();
void config_show();

extern bool config_wants_help;
extern bool config_debug;
extern int config_segment_length;
extern int config_segment_margin;
extern int config_segment_variance;

extern std::vector<std::string> config_data_sources;

