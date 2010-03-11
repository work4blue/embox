/**
 * @file
 * @brief EMBOX testing framework.
 * @details Test is a simple function which examines some parts of hardware or
 * software. Test can be either passed or failed (framework uses return value
 * of test function to determine the test result).
 * Each test resides in a single mod which is usually defined in
 * @code embox.test @code or @code <platform>.test @code packages.
 *
 * @sa EMBOX_TEST() simple macro used to declare a new test
 *
 * @date Jul 4, 2009
 * @author Anton Bondarev, Alexey Fomin
 *         - Initial implementation
 * @author Eldar Abusalimov
 *         - Rewriting from scratch, adapting for usage with mod DI framework
 */

#ifndef EMBOX_TEST_H_
#define EMBOX_TEST_H_

#include <stddef.h>

#include <embox/mod.h>

#define EMBOX_TEST(_run) \
	extern const char MOD_SELF_NAME[]; \
	EMBOX_TEST_DETAILS(_run, MOD_SELF_NAME, NULL, __EMBUILD_MOD__)

#define EMBOX_TEST_DETAILS(_run, _name, _info, _mod) \
	MOD_DEP_DEF(_mod, __test_tag); \
	MOD_DATA_DEF(_mod, __test__); \
	MOD_OPS_DEF(_mod, __test_mod_ops); \
	static int _run(void); \
	static int __test_run__(void) __attribute__ ((alias(#_run))); \
	static struct test_private __test_private__; \
	static const struct test __test__ = { \
			.private = &__test_private__, \
			.run = _run, \
			.info = _info, \
			.name = _name \
		}

#define EMBOX_TEST_EXPORT(symbol) \
	int __test_symbol__##symbol(void) __attribute__ ((alias("__test_run__")))

#define EMBOX_TEST_IMPORT(symbol) \
	static int symbol(void) __attribute__ ((alias("__test_symbol__"#symbol)))

extern struct mod_ops __test_mod_ops;

/**
 * Each test implements this interface.
 *
 * @return the test result
 * @retval 0 on success
 * @retval nonzero on fail (if the test provides @link #test_info_t
 * detailed info @endlink function)
 */
typedef int(*test_run_t)(void);

/**
 * @param result
 */
typedef void (*test_info_t)(int result);

struct test_private;

struct test {
	/** Internal data. */
	struct test_private *private;
	/** The test itself. */
	test_run_t run;
	/** (optional) Shows detailed results of the last test. */
	test_info_t info;
	/** (optional) Short annotation that will be shown on executing test. */
	const char *name;
};

struct test_private {
	int result;
};

#endif /* EMBOX_TEST_H_ */
