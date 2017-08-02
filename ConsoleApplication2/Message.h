#pragma once

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Signal.h"
#include <iostream>

class Message {

public:

	uint32_t MsgSize()const { return m_msg_size; }
	void SetMsgSize(uint32_t msg_size)
	{
		m_msg_size = msg_size;
	}

	const std::string & Msgtransmitter() const { return m_msg_transmitter; }
	void SetMsgtransmitter(const std::string &msg_transmitter)
	{
		m_msg_transmitter = msg_transmitter;
	}

	std::string const & Name() const { return m_message_name; }
	void SetMessageName(std::string const & _name) {
		m_message_name = _name;
	}

	uint32_t ID(void) const { return m_msg_id; }
	void SetMsgID(uint32_t _msg_id) { m_msg_id = _msg_id; }

	void AddSignal(Signal const & _signal) {
		m_signals.push_back(_signal);
	}

	std::vector<Signal> & Signals(void) {
		return m_signals;
	}
	std::vector<Signal> const & Signals(void) const {
		return m_signals;
	}

private:
	std::string         m_message_name;//<message name
	uint32_t            m_msg_id;//< message id
	std::vector<Signal> m_signals;//< signals

	uint32_t				m_msg_size;//<message size
	std::string			m_msg_transmitter;//<message transmitter
};

std::ostream & operator<<(std::ostream & os, const Message & message);

#endif