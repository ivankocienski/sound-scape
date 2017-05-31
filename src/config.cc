#include "config.hh"

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
bool config_wants_help = false;
bool config_debug = false;
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
    ("source,s", po::value< vector<string> >(), "source directory");

  po::variables_map var_map;
  po::store(po::parse_command_line(argc, argv, desc), var_map);
  po::notify(var_map);    

  if (var_map.count("help")) {
    config_wants_help = true;
    return;
  }

  if (var_map.count("debug")) {
    config_debug = true;
  }

  if (var_map.count("source")) {
    config_data_sources = var_map["source"].as< vector<string> >();
  }
}

bool config_is_valid() {

  if(config_data_sources.size() == 0) {
    cerr << "Missing data sources" << endl;
    config_show_help();
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
