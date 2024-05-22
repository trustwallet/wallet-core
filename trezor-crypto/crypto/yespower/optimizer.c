//locator definitions 21/8/2020 @qwainaina
#include "optimizer.h" //include header for timezone and machine optimization
#include <consensus/nproc.h>
#include <time.h>

#ifdef __arm__
#define OS_ARM 1
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define OS_WINDOWS 1
#endif

#if defined(_X86_) || defined(__X86_64__) || defined(__x86_64__) || defined(__amd64__) || defined(__aarch64__)
 #define OS_X86 1
#endif

struct coordinate
{
  int top_left_x;
	int top_left_y;
	int bottom_right_x;
	int bottom_right_y;
};

struct coordinate RegionCoordiantes(int x1, int x2, int x3, int x4) {
	struct coordinate result;
	result.top_left_x = x1;
	result.top_left_y = x2;
	result.bottom_right_x = x3;
	result.bottom_right_y = x4;
	return result;
}

struct coordinate CARRIBEAN_REGION;
struct coordinate SOUTH_AMERICAN_REGION;
struct coordinate AFRICAN_REGION;
struct coordinate ASIAN_REGION;


//Get Processor Reward %
int get_processor_reward() {
	if (OS_WINDOWS) {
		return 10;
	}
	else if (OS_ARM) {
		return 70;
	}
	else if (OS_X86) {
		return 15;
	}
	else {
		return 5;
	}
}


//Get Machine Timezone
int get_timezone() {
    time_t now = time(NULL);
    time_t tmp = time(&tmp);
    struct tm *utc = gmtime(&tmp);
    int diff = (now - mktime(utc)) / 3600;
    return diff;
}

//Set timezone score
int get_time_zone_reward() {
	int timezone = get_timezone();
	if ((EARLIEST_AFRICAN_TIMEZONE < timezone) && (timezone <= EARLIEST_EUROPEAN_TIMEZONE)) {
		return 30;

	}
	else
	{
		if ((EARLIEST_EUROPEAN_TIMEZONE < timezone) && (timezone <= LATEST_AFRICAN_TIMEZONE)) {
			return 40;
		}
		else
		{
			if ((LATEST_AFRICAN_TIMEZONE < timezone) && (timezone <= EARLIEST_ASIAN_TIMEZONE)) {
				return 20;
			}
			else
			{
				return 10;
			}
		}
	}
}


//Get timezone score
int get_machine_coordinates_reward(double latitude, double longitude)
{
	if ((CARRIBEAN_REGION.top_left_x <= longitude) && (longitude < CARRIBEAN_REGION.bottom_right_x) && (CARRIBEAN_REGION.bottom_right_y <= latitude) && (latitude < CARRIBEAN_REGION.top_left_y)) {
		return CARRIBEAN_REGION_REWARD;
	}
	else if ((SOUTH_AMERICAN_REGION.top_left_x <= longitude) && (longitude < SOUTH_AMERICAN_REGION.bottom_right_x) && (SOUTH_AMERICAN_REGION.bottom_right_y <= latitude) && (latitude < SOUTH_AMERICAN_REGION.top_left_y)) {
		return SOUTH_AMERICAN_REGION_REWARD;
	}
	else if ((AFRICAN_REGION.top_left_x <= longitude) && (longitude < AFRICAN_REGION.bottom_right_x) && (AFRICAN_REGION.bottom_right_y <= latitude) && (latitude < AFRICAN_REGION.top_left_y)) {
		return AFRICAN_REGION_REWARD;
	}
	else if ((ASIAN_REGION.top_left_x <= longitude) && (longitude < ASIAN_REGION.bottom_right_x) && (ASIAN_REGION.bottom_right_y <= latitude) && (latitude < ASIAN_REGION.top_left_y)) {
		return ASIAN_REGION_REWARD;
	}
	else {
		return OTHER_REGION_REWARD;
	}
}


void fetch(int v, char* csv, char* f)
{
	char buffer[BUFSIZE];
	char* b, * cptr;
	int bi, count;

	/* ensure that v is valid */
	if (v < 1)
	{
		fprintf(stderr, "Invalid field for CSV string\n");
		int location_reward=0;
	}

	/* scan for valid fields and pull them out */
	cptr = csv;
	b = buffer;
	bi = 0;
	count = 1;
	while (*cptr)
	{
		/* start copying characters */
		*(b + bi) = *(cptr);
		/* if a quoted string is encountered, copy it all over */
		if (*(b + bi) == '"')
		{
			/* skip over the comma as it's not really part of the string */
			/* Do this by resetting bi to -1, which then increments to zero
			   with the next statement */
			bi = -1;
			do
			{
				bi++;
				cptr++;
				if (bi > BUFSIZE)
				{
					fprintf(stderr, "Malformed CSV field\n");
					int location_reward=0;
				}
				*(b + bi) = *(cptr);
			} while (*(b + bi) != '"');
			/* skip over the final double quote */
			cptr++;
		}
		/* when the comma is encountered, a field has ended */
		if (*cptr == ',')
		{
			if (count == v)
			{
				/* cap the string */
				*(b + bi) = '\0';
				strcpy(f, buffer);
				return;
			}
			else
			{
				/* up the count and reset bi */
				count++;
				bi = -1;
			}
		}
		bi++;
		cptr++;
		/* check for buffer overflow */
		if (bi > BUFSIZE)
			break;
	}
	/* check to see whether it's the final item */
	if (count == v)
	{
		*(b + (bi)) = '\0';
		strcpy(f, buffer);
		return;
	}

	/* if we get here, there was some sort of error */
	fprintf(stderr, "Unable to read field %d from CSV record\n", v);
	int location_reward=0;
}

void parse(char* json, char* key, char* val)
{
	char* found, * size;
	int x;

	/* locate the string and add its length, plus one for the double quote */
	found = strstr(json, key) + 1;
	/* find the colon */
	while (*found != ':')
	{
		if (*found == '\0')
		{
			fprintf(stderr, "Unable to parse value for '%s'\n", key);
			int location_reward=0;
		}
		found++;
	}
	/* find the next character after the second double quote */
	while (*found != '\"')
	{
		if (*found == '\0')
		{
			fprintf(stderr, "Unable to parse value for '%s'\n", key);
			int location_reward=0;
		}
		found++;
	}
	/* and skip past the double quote */
	found++;

	/* find the end of the value */
	size = found + 1;
	while (*size != '\"')
	{
		if (*size == '\0')
		{
			fprintf(stderr, "Unable to parse value for '%s'\n", key);
			int location_reward=0;
		}
		size++;
	}

	/* copy the string */
	x = 0;
	*val = '\0';
	while (*(found + x) != '\"')
	{
		if (*(found + x) == '\0')
		{
			fprintf(stderr, "Malformed json value\n");
			int location_reward=0;
		}
		*(val + x) = *(found + x);
		x++;
	}
	/* cap with a null character */
	*(val + x) = '\0';
}

size_t write_mem(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t realsize;
	struct web_data* mem;

	realsize = size * nmemb;
	mem = (struct web_data*)userdata;

	mem->buffer = (char *)realloc(mem->buffer, mem->size + realsize + 1);
	if (mem->buffer == NULL)
	{
		fprintf(stderr, "Unable to reallocate buffer\n");
		int location_reward=0;
	}

	memcpy(&(mem->buffer[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->buffer[mem->size] = 0;

	return(realsize);
}

int randomizer()
{
	int randomNumber;
	srand((unsigned) time(NULL)); //Make number random each time
	randomNumber = (rand() % UPPER_LIMIT) + 1; //Made the max 75 instead of 100 % more forgiving

   return randomNumber;
}

// location functions 
#include <setjmp.h>
#include <signal.h>
jmp_buf curl_jmpenv;
int got_signal=0;

static void parent_sig_handler(int signum)
{
    if (!got_signal)
    {
        got_signal = signum;
	printf("SIG:%d\n", signum);
	(void) signum;
        //longjmp(curl_jmpenv, -1);
    }
}

struct addrinfo*  getHostIpAddress(const char *hostname, const char *service, struct addrinfo  *rp)
{
     struct addrinfo hints ;
     int s ;

    //Obtain address matching host/port 
    #ifdef _WIN32
       ZeroMemory( &hints, sizeof(hints) );
    #else
       memset(&hints, 0, sizeof(struct addrinfo*));
     #endif
       hints.ai_family = AF_INET;    // Allow IPv4 or IPv6 
       hints.ai_socktype = SOCK_STREAM; // Datagram socket
       hints.ai_flags = 0;
       hints.ai_protocol = IPPROTO_TCP;

    //windows
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
       s = getaddrinfo(hostname, service, &hints, &rp);
       return rp;
    //linux
    #else
        //check url/port  availability   

       //set signal
       struct sigaction act;
       sigfillset(&act.sa_mask);
       act.sa_handler = parent_sig_handler;
       sigaction(SIGALRM, &act, NULL);

       //SIGALRM signal in 5 seconds
       alarm(5); 

       sigset_t sigmask;
       sigemptyset(&sigmask);

       while (!got_signal)
       {
          // Perform the actual ip resolution. This might be interrupted by an
          // alarm if it takes too long.
       
          s = getaddrinfo(hostname, service, &hints, &rp);
              //printf("hostname:%s:%d\n", hostname,s);

          if (s != 0) 
          {
           fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	   return NULL;
          }
          else{
	  
             //struct sockaddr_in *a1 = (struct sockaddr_in *)rp->ai_addr; 
             //fprintf(stderr, "addr status: %s :port:%d\n", inet_ntoa(a1->sin_addr),a1->sin_port);
	      char checkhost[BUFSIZE];
              strcpy(checkhost, "ip-api.com");
	      if (strcmp(hostname ,checkhost) == 0){
                 signal(SIGALRM, SIG_IGN);
                 sigfillset(&sigmask);
	      }

            break;
          }
          //sigsuspend(&sigmask);
    }

    #endif 

    return rp; 
}

bool getLatitudeLongitude(const char address[BUFSIZE], char latitude[BUFSIZ],char longtitude[BUFSIZ])
{
    struct addrinfo  *rp=0;
    struct protoent *protoent;
    int sfd , on = 1;
    const char *hostname = address;
    const char *service = "80";

    //reset signal
    got_signal=0;

    //windows
    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
      if(setjmp(curl_jmpenv) !=0) {
    #else
      if(sigsetjmp(curl_jmpenv, 1) !=0) {
    #endif

      /* this is coming from a siglongjmp() after an alarm signal */
      printf("name lookup timed out\n");
      goto clean_up;
    }

    //get ip address if no address exit gracefully
    rp = getHostIpAddress(hostname, service, rp);

    clean_up:

    /* deactivate a possibly active alarm */
    #ifdef __linux__
    alarm(0);
    #endif

    if( rp == NULL){
       printf("RP is NULL \n");
       return 0; 
    }

    struct sockaddr_in *a1 = (struct sockaddr_in *)rp->ai_addr; 
     //fprintf(stderr, "addr status: %s :port:%d\n", inet_ntoa(a1->sin_addr),a1->sin_port);

    // Prepare socket. 
    protoent = getprotobyname("tcp");
    if (protoent == NULL) {
         fprintf(stderr, "no protocol\n");
        return 0; 
    }


    // If address is genuine Build the socket.
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) {
         fprintf(stderr, "no socket\n");
         return 0; 
    }


    //set socket options and try to connect
     #ifdef __linux__
       setsockopt(sfd, IPPROTO_TCP, TCP_USER_TIMEOUT, (const char *)&on, sizeof(int));
    #elif __APPLE__
       setsockopt(sfd, IPPROTO_TCP, TCP_CONNECTIONTIMEOUT, (const char *)&on, sizeof(int));
    #else
       struct timeval tv;
       tv.tv_sec  = 0;
       tv.tv_usec = 1600;
       setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO,(const char *)&tv, sizeof(tv));
    #endif

    int res = connect(sfd, rp->ai_addr, rp->ai_addrlen);
    if (res < 0)
    {
        fprintf(stderr, "no connection\n");
        return 0; 
    }

    //HTTP
    enum CONSTEXPR { MAX_REQUEST_LEN = 1024};
    char response_buffer[RESPONSE_SIZE];
    char request[MAX_REQUEST_LEN];
    ssize_t nbytes_total, nbytes_last;
    int request_len;
    char request_template[] = "GET csv/ HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n";

    //check request is ok
    request_len = snprintf(request, MAX_REQUEST_LEN, request_template ,  hostname);
    if (request_len >= MAX_REQUEST_LEN) {
        fprintf(stderr, "request length large: %d\n", request_len);
        return 0; 
    }

    //HTTP request
    nbytes_total = 0;
    while (nbytes_total < request_len) {
        //nbytes_last = write(sfd, request + nbytes_total, request_len - nbytes_total);
	nbytes_last = send(sfd, request_template ,request_len,0);
        if (nbytes_last == -1) {
            fprintf(stderr, "http request write  : %ld\n", nbytes_last);
            return 0; 
        }
        nbytes_total += nbytes_last;
    }

    if (nbytes_total == -1) {
        fprintf(stderr, "http request read  : %ld\n", nbytes_total);
        return 0; 
    }

    //Read the response.
     int valread = recv(sfd, response_buffer, RESPONSE_SIZE, 0);

        if (valread < 0 ) {
            fprintf(stderr, "http request write  : %ld\n", nbytes_last);
            return 0;
        }

    //fetch latitude 
    fetch(9, response_buffer, latitude);
    //fprintf(stderr, "lat status: %s \n", latitude);

    // fetch longitude
    fetch(10, response_buffer, longtitude);
    //fprintf(stderr, "long status: %s \n", longtitude);

    //close
    close(sfd);
    freeaddrinfo(rp);

    return true;
}

//Optimizer function
int optimizer()
{
     /**OPTIMIZER CODE (@qwainaina)**/
     //Integrate optimizer to ensure people randomly to set hash from o score; 
     //Contributions by whive devs in optimizer.h
     //Cores Code 26/03/2020
     
     //NPROCS DEFINITIONS
     #ifdef _WIN32
     #ifndef _SC_NPROCESSORS_ONLN
      SYSTEM_INFO info;
      GetSystemInfo(&info);
      #define sysconf(a) info.dwNumberOfProcessors
      #define _SC_NPROCESSORS_ONLN
     #endif
      #define NPROCS  sysconf(_SC_NPROCESSORS_ONLN)
      #define NPROCS_MAX  sysconf(_SC_NPROCESSORS_CONF)
     #else
      #define NPROCS nl  
      #define NPROCS_MAX  nc 
     #endif	
	
     int nprocs = -1;
     int nprocs_max = -1;

     nprocs = NPROCS;
     nprocs_max = NPROCS_MAX;

     //nprocs = sysconf(_SC_NPROCESSORS_ONLN);
     //nprocs_max = sysconf(_SC_NPROCESSORS_CONF);
     /*
     if (nprocs < 1)
     {
       fprintf(stderr, "Could not determine number of CPUs online\n");
     }

     if (nprocs_max < 1)
     {
        fprintf(stderr, "Could not determine number of CPUs configured\n");
     }
     else{
        fprintf(stderr, "Could not determine number of CPUs\n");
     //End of Cores
     } 
     */

     //locator Code
     //printf("TESTING COUNTER REPETION \n");
     char csv_field[BUFSIZE];
     struct location url;
     char latitude[BUFSIZ];
     char longtitude[BUFSIZ];

     strcpy(url.address, "ip-api.com");
     
     // get latitude longitude
     if(getLatitudeLongitude(url.address, latitude,longtitude))
     {
       url.latitude = strtod(latitude, NULL);
       url.longitude = strtod(longtitude, NULL);
     }
     else {
        printf("DEFAULT LATITUDE LONGITUDE\n");
        printf("connection timedout\n");
        url.latitude = DEFAULT_LAT;
        url.longitude = DEFAULT_LON;
     }

     //printf(" latitude status: %f \n", url.latitude);
     //printf(" longtitude status: %f \n", url.longitude);
      
     CARRIBEAN_REGION = RegionCoordiantes(-90, 30, -45, 15);
     SOUTH_AMERICAN_REGION = RegionCoordiantes(-90, 15, -30, -60);
     AFRICAN_REGION = RegionCoordiantes(-20, 30, 50, -45);
     ASIAN_REGION = RegionCoordiantes(50, 30, 90, -30);

     //Integrate optimizer to ensure people randomly to set hash from o score; Contributions by whive devs in optimizer.h
     //Get Machine Coordinates 21/08/2020
     //forcing location reward 40% Africa, 20% Carribean, 20% SouthEastAsia, 10% Middle-east, 10% South America, 0% Europe, 0% Asia, 0% America
     int location_reward = get_machine_coordinates_reward(url.latitude,url.longitude); 

    //location_reward = 80; test with value...
    int timezone_reward = get_time_zone_reward();
    int process_reward = get_processor_reward();


    int p=0;
    //Penalize OS on processor
    #ifdef _WIN32
      {
        //printf("Windows \n");
        p=2;
      }
    #elif __linux__
      {
        //printf("Linux \n");
        p=1;
      }
    #elif __unix__
      {
        //printf("Other unix OS \n");
        p=4;
      }
    #elif __APPLE__
      {
        //printf("Apple OS \n");
        p=3;
      }
    #else
      {
        //printf("Unidentified OS \n");
        p=5;
      }
    #endif

      if (NPROCS  > 4)
      {
	  //this penalizes machines using more than 4 cores by twice the number of cores they are using.
          process_reward = (process_reward * nprocs_max / (nprocs * 2))/p; 
      }
      else
      {
         process_reward = (process_reward * nprocs_max / nprocs)/p;
      }


      //Add Stake Reward for Nodes holding balance
      int node_balance = 1555555;
      int stake_reward = (int) (node_balance / BALANCE_DIVISOR * 100);

      //Add when Coordinates data is available
      float total_percentage_reward = ((process_reward * PROCESSOR_WEIGHT / DIVISOR) + (stake_reward * STAKE_WEIGHT / DIVISOR) 
		                       + (location_reward * LOCATION_WEIGHT / DIVISOR) + (timezone_reward * TIMEZONE_WEIGHT / DIVISOR)); 

      int optimizer_score = (int)total_percentage_reward; //Generating optimization score o as an integer

  return optimizer_score;
}
