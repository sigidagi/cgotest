#pragma once

#include <cstdlib>
#include <fmt/core.h>
//#include <nlassert-nonproduction.h>

/**
 * Base-level abnormal termination.
 *
 * Terminate the program immediately, without invoking destructors, atexit callbacks, etc.
 * Used to implement the default `chipDie()`.
 *
 * @note
 *  This should never be invoked directly by code outside this file.
 */
#if !defined(CHIP_CONFIG_ABORT)
#define CHIP_CONFIG_ABORT() abort()
#endif


#ifndef chipAbort
extern "C" void chipAbort(void) __attribute((noreturn));

inline void chipAbort(void)
{
    while (true)
    {
        // NL_ASSERT_ABORT is redefined to be chipAbort, so not useful here.
        CHIP_CONFIG_ABORT();
    }
}
#endif // chipAbort
#ifndef chipDie
extern "C" void chipDie(void) __attribute((noreturn));

inline void chipDie(void)
{
    //ChipLogError(NotSpecified, "chipDie chipDie chipDie");
    chipAbort();
}
#endif // chipDie

/**
 *  @def VerifyOrDie(aCondition)
 *
 *  @brief
 *    This checks for the specified condition, which is expected to
 *    commonly be true and forces an immediate abort if the condition
 *    is false.
 *
 *  Example Usage:
 *
 *  @code
 *  void FreeBuffer(const uint8_t *buf)
 *  {
 *      VerifyOrDie(buf != NULL);
 *      free(buf);
 *  }
 *  @endcode
 *
 *  @param[in]  aCondition  A Boolean expression to be evaluated.
 *
 *  @sa #VerifyOrDieWithMsg
 *  @sa #chipDie
 *
 */
#if CHIP_CONFIG_VERBOSE_VERIFY_OR_DIE
#define VerifyOrDie(aCondition)                                                                                                    \
    nlABORT_ACTION(aCondition, fmt::print("VerifyOrDie failure at {}:{}: {}", __FILE__, __LINE__, #aCondition))
#else // CHIP_CONFIG_VERBOSE_VERIFY_OR_DIE
#define VerifyOrDie(aCondition) nlABORT(aCondition)
#endif // CHIP_CONFIG_VERBOSE_VERIFY_OR_DIE

/**
 *  @def VerifyOrDieWithMsg(aCondition, aModule, aMessage, ...)
 *
 *  @brief
 *    This checks for the specified condition, which is expected to
 *    commonly be true and both prints @a aMessage and forces an
 *    immediate abort if the condition is false.
 *
 *  Example Usage:
 *
 *  @code
 *  void FreeBuffer(const uint8_t *buf)
 *  {
 *      VerifyOrDieWithMsg(buf != NULL, MemoryManagement, "Invalid pointer passed to FreeBuffer");
 *      free(buf);
 *  }
 *  @endcode
 *
 *  @param[in]  aCondition  A Boolean expression to be evaluated.
 *  @param[in]  aModule     A chip LogModule short-hand mnemonic identifing
 *                          the logical section of code that is a
 *                          source the logged message.
 *  @param[in]  aMessage    A pointer to a NULL-terminated C string with
 *                          C Standard Library-style format specifiers
 *                          containing the log message to be formatted
 *                          and logged.
 *  @param[in]  ...         A variadic argument list whose elements should
 *                          correspond to the format specifiers in @a
 *                          aMessage.
 *
 *  @sa #VerifyOrDie
 *  @sa #chipDie
 *
    nlABORT_ACTION(aCondition, fmt::print("DieWithMessage: {}", aMessage, ##__VA_ARGS__))
    // TODO
 */
#define VerifyOrDieWithMsg(aCondition, aModule, aMessage, ...)                                                                     \
    fmt::print("DieWithMessage: {}", aMessage, ##__VA_ARGS__)



#define VerifyOrReturn(expr, ...)                                                                                                  \
    do                                                                                                                             \
    {                                                                                                                              \
        if (!(expr))                                                                                                               \
        {                                                                                                                          \
            __VA_ARGS__;                                                                                                           \
            return;                                                                                                                \
        }                                                                                                                          \
    } while (false)

/**
 *  @def VerifyOrReturnError(expr, code, ...)
 *
 *  @brief
 *    Returns a specified error code if expression evaluates to false
 *
 *  Example usage:
 *
 *  @code
 *    VerifyOrReturnError(param != nullptr, CHIP_ERROR_INVALID_ARGUMENT);
 *  @endcode
 *
 *  @param[in]  expr        A Boolean expression to be evaluated.
 *  @param[in]  code        A value to return if @a expr is false.
 *  @param[in]  ...         Statements to execute before returning. Optional.
 */
#define VerifyOrReturnError(expr, code, ...) VerifyOrReturnValue(expr, code, ##__VA_ARGS__)

/**
 *  @def VerifyOrReturnValue(expr, value, ...)
 *
 *  @brief
 *    Returns a specified value if expression evaluates to false
 *
 *  Example usage:
 *
 *  @code
 *    VerifyOrReturnError(param != nullptr, Foo());
 *  @endcode
 *
 *  @param[in]  expr        A Boolean expression to be evaluated.
 *  @param[in]  value       A value to return if @a expr is false.
 *  @param[in]  ...         Statements to execute before returning. Optional.
 */
#define VerifyOrReturnValue(expr, value, ...)                                                                                      \
    do                                                                                                                             \
    {                                                                                                                              \
        if (!(expr))                                                                                                               \
        {                                                                                                                          \
            __VA_ARGS__;                                                                                                           \
            return (value);                                                                                                        \
        }                                                                                                                          \
    } while (false)

/**
 *  @def ReturnErrorCodeIf(expr, code)
 *
 *  @brief
 *    Returns a specified error code if expression evaluates to true
 *
 *  Example usage:
 *
 *  @code
 *    ReturnErrorCodeIf(state == kInitialized, CHIP_NO_ERROR);
 *    ReturnErrorCodeIf(state == kInitialized, CHIP_ERROR_INCORRECT_STATE);
 *  @endcode
 *
 *  @param[in]  expr        A Boolean expression to be evaluated.
 *  @param[in]  code        A value to return if @a expr is false.
 */
#define ReturnErrorCodeIf(expr, code)                                                                                              \
    do                                                                                                                             \
    {                                                                                                                              \
        if (expr)                                                                                                                  \
        {                                                                                                                          \
            return code;                                                                                                           \
        }                                                                                                                          \
    } while (false)



static constexpr bool IsSuccess(int error) { return error == 0; }
#define SuccessOrExit(aStatus) nlEXPECT(::IsSuccess((aStatus)), exit)

#define VerifyOrExit(aCondition, anAction) nlEXPECT_ACTION(aCondition, exit, anAction)

/**
 *  @def ReturnErrorOnFailure(expr)
 *
 *  @brief
 *    Returns the error code if the expression returns an error. For a CHIP_ERROR expression, this means any value other
 *    than CHIP_NO_ERROR. For an integer expression, this means non-zero.
 *
 *  Example usage:
 *
 *  @code
 *    ReturnErrorOnFailure(channel->SendMsg(msg));
 *  @endcode
 *
 *  @param[in]  expr        An expression to be tested.
 *
 */

#define ReturnErrorOnFailure(expr)                                                                                                 \
    do                                                                                                                             \
    {                                                                                                                              \
        auto __err = (expr);                                                                                                       \
        if (!::IsSuccess(__err))                                                                                  \
        {                                                                                                                          \
            return __err;                                                                                                          \
        }                                                                                                                          \
    } while (false)


