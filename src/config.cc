#include "config.hh"
#include "misc.hh"

#include <iostream>
#include <algorithm>

#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

// 'public' variables
bool config_wants_help       = false;
bool config_debug            = false;

int  config_segment_length   = 100;
int  config_segment_margin   = 40;
int  config_segment_variance = 20;

vector<string> config_data_sources;

// 'private' variables
static po::options_description desc("Allowed options");

/*
 
 -- help
 -- source, -s

 */

void config_parse_args(int argc, char ** argv) {

  desc.add_options()
    ("help", "show help/about")
    ("debug", "Output verbose debugging information")
    ("segment-ms,d", po::value<int>(), "Length of segments in milliseconds") 
    ("segment-variance-ms,a", po::value<int>(), "Vary the length of segments by at most this amount")
    ("segment-margin-ms,m", po::value<int>(), "Length of fade in/out on a segment")
    ("source,s", po::value< vector<string> >(), "source directory");

  po::variables_map var_map;
  po::store(po::parse_command_line(argc, argv, desc), var_map);
  po::notify(var_map);    

  if (var_map.count("help")) {
    config_wants_help = true;
    return;
  }

  if (var_map.count("debug")) 
    config_debug = true;

  if (var_map.count("segment-ms")) {
    //cout << "var_map='" << var_map["segment-ms"].as<string>() << "'" << endl;
    config_segment_length = var_map["segment-ms"].as<int>();
  }

  if (var_map.count("segment-variance-ms")) 
    config_segment_variance = var_map["segment-variance-ms"].as<int>();
  
  if (var_map.count("segment-margin-ms")) 
    config_segment_margin = var_map["segment-margin-ms"].as<int>();

  if (var_map.count("source"))
    config_data_sources = var_map["source"].as< vector<string> >();
}

void config_show() {

  debug("config.segment_length=%d", config_segment_length);
  debug("config.segment_margin=%d", config_segment_margin);
  debug("config.segment_variance=%d", config_segment_variance);

}

bool config_is_valid() {

  if(config_data_sources.size() == 0) {
    cerr << "Missing data sources" << endl;
    config_show_help();
    return false;
  }

  if(config_segment_length < 10) {
    cerr << "segments must be at least 10 milliseconds long" << endl;
    return false;
  }

  if(config_segment_margin > (config_segment_length / 2)) {
    cerr << "segment margin must be less that half the length of segment" << endl;
    return false;
  }

  
  for( auto & it : config_data_sources ) {
    if(fs::is_directory(fs::status(fs::path(it)))) continue;

    cerr << "Source " << it << " does not appear to exist" << endl;

    return false;
  }

  // prune duplicates
  auto it = unique(config_data_sources.begin(), config_data_sources.end()); 
  config_data_sources.resize(distance(config_data_sources.begin(), it));

  return true; 
}

void config_show_help() {

  cout << "Help" << endl;

  cout << desc << endl;
}
