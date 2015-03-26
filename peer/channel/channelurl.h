//
// channelurl.h
//

#ifndef	__CHANNEL_URL_H__
#define	__CHANNEL_URL_H__

#include "os.h"

class ChannelUrl
{
public:
	ChannelUrl(const std::string& url);
	virtual ~ChannelUrl();
    
    std::string cid() const;
    
private:
    std::string _cid;
};

#endif 
