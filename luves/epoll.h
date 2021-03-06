//
//  kqueue.h
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifdef __linux__

#ifndef EPOLL_H_
#define EPOLL_H_

#include <sys/epoll.h>
#include <vector>
#include <cstring>
#include <map>
#include "channel.h"
#include "eventhandle.h"
#include "threadpool.h"


namespace luves
{
	class Channel;

	typedef std::vector<Channel *> ChannelList;
	typedef std::map<int, Channel*> ChannelMap;

	class EpollModel
	{
		public:
			EpollModel();
			~EpollModel(){};

			//事件通道操作，只暴露给事件循环EventLoop模块操作
			void AddChannel(Channel * channel);
			void DeleteChannel(Channel * channel);
			void UpdateChannel(Channel * channel);

			//启动事件模型
			void RunModel(int64_t wait_time);

			void SetChannelPtr(std::map<int ,Channel*> * channel_fd){ channel_fd_= channel_fd; }
			ChannelList  & GetTriggerPtr();

			void SetHsha(bool is_hsha){is_hsha = is_hsha_;}
		private:
			bool is_hsha_;
			struct epoll_event trigger_events_[1024];
			std::map<int ,Channel*> * channel_fd_;	   //channel与fd映射
			ChannelList channel_list_;                 //channel集合
			ChannelList trigger_channel_;              //触发的channel集合
			int ep_;
			int max_events_;
			int listen_fd_;
	};
}


#endif /* kqueue.h */

#endif /* LINUX */
