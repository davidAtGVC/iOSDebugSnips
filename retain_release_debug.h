/*
	retain_release_debug.h
	http://rentzsch.tumblr.com/post/3895140826/retain-release-debugging
	Stick it into a class and out will print a nice stack trace for each invocation of retain and release for each instance of your class, helping you spot imbalances. I keep it in an #if 1 block so it’s easy to turn on and off across debugging runs (usually I paste this code into two or three classes at a time to catch subtle interactions that are causing the bug). There are higher-level tools for debugging this stuff, but this is quick and easy to inject into specific troublesome classes, find the issue and then back it out again. Usually my use for these don’t even make it into version control. Plus, its printouts are real pretty thanks to +[NSThread callStackSymbols].

	Modified into macro by
	David Aspinall, Global Village Consulting, Mar 16, 2011
 
*/

/** class implementation */

#if DEBUG

/*
	To use: simply inject the RETAIL_RELEASE_DEBUG into the class or classes you are debugging.  If you forget to remove them they will not be included in production/Release builds, only targets with the DEBUG set.
 */

#define GVC_RETAIN_RELEASE_DEBUG	\
- (id)retain { \
	NSUInteger oldRetainCount = [super retainCount]; \
	id result = [super retain]; \
	NSUInteger newRetainCount = [super retainCount]; \
	printf( "%s<%p> ++retainCount: %d => %d\n", [NSStringFromClass([self class]) UTF8String], self, oldRetainCount, newRetainCount); \
	printf( "%s\n", [[[NSThread callStackSymbols] description] UTF8String] ); \
	return result; \
} \
- (void)release { \
    NSUInteger oldRetainCount = [super retainCount]; \
    BOOL gonnaDealloc = oldRetainCount == 1; \
    if (gonnaDealloc) { \
        printf( "%s<%p> --retainCount: 1 => 0 (gonna dealloc)\n", [NSStringFromClass([self class]) UTF8String], self); \
        printf( "%s\n", [[[NSThread callStackSymbols] description] UTF8String]); \
    } \
    [super release]; \
    if (!gonnaDealloc) { \
        NSUInteger newRetainCount = [super retainCount]; \
		printf( "%s<%p> --retainCount: %d => %d\n", [NSStringFromClass([self class]) UTF8String], self, oldRetainCount, newRetainCount); \
		printf( "%s\n", [[[NSThread callStackSymbols] description] UTF8String] ); \
    } \
} 

#else

#define GVC_RETAIN_RELEASE_DEBUG	/* does nothing in production */

#endif
