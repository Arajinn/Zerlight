//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_EXCEPTION_H
#define GUICHAN_EXCEPTION_H

#include <string>

#ifndef __FUNCTION__
#define __FUNCTION__ "?"
#endif

/*
 * A macro used to create a standard exception object.
 * What it basicly does is that it creates a new exception
 * and automatically sets the filename and line number where
 * the exception occured by using other compiler macros.
 */
#define GCN_EXCEPTION(mess) gui::Exception(mess,   \
                            __FUNCTION__,          \
                            __FILE__,              \
                            __LINE__)

namespace gui {
    /**
     * An exception containing a message, a file and a line number
     * where the exception occured. Guichan will only throw exceptions
     * of this class.
     *
     * You can use this class for your own exceptions that has
     * something to do with a GUI exception. A nifty feature of the
     * excpetion class is that it can tell you from which line and
     * file it was thrown. To make things easier when throwing
     * exceptions there exists a macro for creating exceptions
     * which automatically sets the filename and line number.
     *
     * EXAMPLE: @code
     *          throw GCN_EXCEPTION("my error message");
     *          @endcode
     */
    class Exception {
    public:

        /**
         * Constructor.
         */
        Exception();

        /**
         * Constructor.
         *
         * @param message The error message of the exception.
         */
        Exception(const std::string& message);

        /**
         * Constructor.
         *
         * NOTE: Don't use this constructor. Use the GCN_EXCEPTION macro instead.
         *       This constructor merely exists for the GCN_EXCEPTION macro to
         *       use.
         *
         * @param message The error message of the exception.
         * @param function The function name where the exception occured.
         * @param filename The name of the file where the exception occured.
         * @param line The line number in the source code where the exception
         *             occured.
         */
        Exception(const std::string& message,
                  const std::string& function,
                  const std::string& filename,
                  unsigned int line);

        /**
         * Gets the function name where the exception occured.
         *
         * @return The function name where the exception occured.
         */
        const std::string& getFunction() const;

        /**
         * Gets the error message of the exception.
         *
         * @return The error message of the exception.
         */
        const std::string& getMessage() const;

        /**
         * Gets the filename where the exception occured.
         *
         * @return The filename where the exception occured.
         */
        const std::string& getFilename() const;

        /**
         * Gets the line number where the exception occured.
         *
         * @return The line number where the exception occured.
         */
        unsigned int getLine() const;

    protected:
        /**
         * Holds the name of the function name where the
         * exception occured.
         */
        std::string mFunction;

        /**
         * Holds the error message of the exception.
         */
        std::string mMessage;

        /**
         * Holds the filename where the exception occured.
         */
        std::string mFilename;

        /**
         * Holds the line number where the exception occured.
         */
        unsigned int mLine;
    };
}

#endif //GUICHAN_EXCEPTION_H
