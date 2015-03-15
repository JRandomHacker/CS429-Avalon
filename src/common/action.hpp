#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>

/**
 * Parent action class for all other actions
 *
 * @class Action
 * @author Matt Hoffman
 * @date 2015-02-10
 */
class Action {
    public:
        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         */
         Action(std::string mess_text);

         /**
          * Destructor
          */
         virtual ~Action();

         /**
          * Getter
          *
          * @return The string representing this Action
          */
         std::string getMessage();

    private:
        std::string message_text;
};

#endif // ACTION_HPP
