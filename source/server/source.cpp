// 
// source.cpp
//

#include "logger.h"
#include "settings.h"
#include "netservermgr.h"
#include "os.h"

void usage()
{
    printf("PPCast Source Server, Copyright 2011-2014 Limlabs.\n"
           "Usage: ppsource [-h] [-v] [-s]\n"
           "\t -h  : help information\n"
           "\t -v  : version\n"
           "\t -s  : running as service (daemon)\n"
           );
}

void version()
{
    printf("PPCast Source Server, Copyright 2011-2014 Limlabs.\n"
           "\t version : %s\n"
           "\t Build   : %s %s\n",
           PPCAST_SOURCE_VERSION_STRING,
           __DATE__, __TIME__
           );
}

// Signals
bool quit = false;
void sig(int v_sig)
{
    quit = true;
}

int run()
{
    
#if defined(__linux__)
    // grow our pool of file descriptors to the max!
    struct rlimit rl;
    // set it to the absolute maximum that the operating system allows
    // have to be superuser to do this
    rl.rlim_cur = 8192;
    rl.rlim_max = 8192;
    setrlimit(RLIMIT_NOFILE, &rl);
    
    // Signals
	signal(SIGTERM,sig); // Termination signal
	signal(SIGUSR1,sig); // User-defined signal 1
	signal(SIGUSR2,sig); // User-defined signal 2
    
	// Ignore signals
	signal(SIGTTOU,SIG_IGN); //Terminal output for background process
	signal(SIGTTIN,SIG_IGN); //Terminal input for background process
	signal(SIGTSTP,SIG_IGN); //Stop typed at terminal
	signal(SIGPIPE,SIG_IGN); //Broken pipe: write to pipe with no readers
#endif
    
    // Load settings
    theSettings.Load();
    
    // Init socket
    OSInitialSocket();
    
    //Start Server
    if(theServerMgr.Start() == false)
    {
        theLogger.Message(MSG_SYSTEM, "Fail to start net server");
        return -1;
    }
    
    // Signal
    signal(SIGINT,sig); // Interrupt from keyboard
    
    // Wait for terminate
    while(quit == false)
    {
        OSSleep(1000);
    }
    
    // Exit
    theServerMgr.Stop();
    OSUninitialSocket();
    theLogger.Message(MSG_SYSTEM,"Stop server");
    
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i],"-h") == 0) // help
            {
                usage();
                return 0;
            }
            else if (strcmp(argv[i],"-v") == 0) // version
            {
                version();
                return 0;
            }
            else if (strcmp(argv[i],"-s") == 0) // running as service/daemon
            {
#if defined(__linux__)
                // Keep working dir
                // Redirect input/output/error to /dev/null.
                if(daemon(1,0) != 0)
                {	
                    printf("Change to daemon failed\n");
                    return -1;
                }
#endif
            }
        }
    }

    version();
    return run();
}
