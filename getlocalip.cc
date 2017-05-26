#include <string>
#include <iostream>

#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

const std::string& localIP()  
{        
	static std::string localip = "";
	if (localip != "") return localip;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);    
    if (fd >= 0)    
    {    
    	char *ip = NULL;  

    	int MAXINTERFACES=16;  
    	struct ifreq buf[MAXINTERFACES];    
    	struct ifconf ifc;    
        ifc.ifc_len = sizeof(buf);    
        ifc.ifc_buf = (caddr_t)buf;    
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))    
        {    
            int intrface = ifc.ifc_len / sizeof(struct ifreq);    

            while (intrface-- > 0)    
            {    
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[intrface])))    
                {    
                    ip = (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));    
                    break;  
                }                        
            }  
        }    
        close (fd);    
        if (ip) localip = ip;    
    }  

	return localip;
} 

int main()
{
	std::cout << localIP() << std::endl;
	std::cout << localIP() << std::endl;
	std::cout << localIP() << std::endl;
	std::cout << localIP() << std::endl;
	std::cout << localIP() << std::endl;
}
