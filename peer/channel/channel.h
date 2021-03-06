//
// channel.h
//

#ifndef	__CHANNEL_H__
#define	__CHANNEL_H__

#include "selector.h"
#include "channelurl.h"

class RtspSession;
class SourceClient;

class Channel : public Selector
{
public:
	Channel(const ChannelUrl& url);
	virtual ~Channel();
    
    bool Start();
    void Stop();
    bool stopped() const;
    
    std::string media() const;
    std::string sdp() const;
    
    // Override from Selector
    virtual void OnRun();

protected:
    // 
    ChannelUrl _url;
    bool _stopped;
    std::string _sdp;
    
    // As a client of source server
    SourceClient* _sourceClient;
};

#endif 

