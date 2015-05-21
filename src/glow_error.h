#ifndef GLOW_ERROR_H_
#define GLOW_ERROR_H_

/*!
 * \return returns a description of the last error that happened
 *         (actually the last message that was set using
 *         \link glow_set_last_error)
 */
const char* glow_get_last_error(void);


/*!
 * \brief set the last error message
 * \param new_message the new error message that will be returned by
 *        \link glow_get_last_error (should not be longer than 2047 characters)
 */
void glow_set_last_error(const char* new_message);


#endif // GLOW_ERROR_H_

