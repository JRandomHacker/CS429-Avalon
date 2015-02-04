#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>

class Action {
public:
	/**
	 * Public constructor
	 * 
	 * @param mess_text The human readable message explaining the contents
	 *     of the action
	 */
	 Action(std::string mess_text);

private:
	std::string message_text;
};

#endif // ACTION_HPP