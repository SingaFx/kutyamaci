#include <cstdio>
#include <string>
#include <boost/program_options.hpp>

#include "database.h"
#include "updater.h"
#include "import_updater.h"
#include "live_updater.h"

using namespace std;
namespace po = boost::program_options;

Database* database=NULL;
Updater* updater=NULL;

int main(int argc, char *argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("live", po::value<string>(), "live import from specified path")
		("import", po::value<string>(), "folder import from specified path")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	////TODO:command line arguments for this!
	database = new Database("127.0.0.1", "root", "maratsafin", "kutya");
	printf("%s\n", database->query("show tables").c_str());

	if (vm.count("live"))
	{
		printf("Live mode!\n");
		printf("The specified directory: %s\n", vm["live"].as<string>().c_str());
		updater = new LiveUpdater(vm["live"].as<string>(), database);
	}
	else if (vm.count("import"))
	{
		printf("Import mode!\n");
		printf("The specified directory: %s\n", vm["import"].as<string>().c_str());
        cout << vm["import"].as<string>();
        cout << "Hello!";
		updater = new ImportUpdater(vm["import"].as<string>(), database);
	}

	if (updater != NULL)
	{
		updater->run();
	}

	//return 0;
}