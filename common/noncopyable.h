#ifndef __HULUZY_COMMON_NONCOPYABLE_H__
#define __HULUZY_COMMON_NONCOPYABLE_H__

namespace huluzy {

namespace noncopyable_
{
	class noncopyable {
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:
		noncopyable(const noncopyable&);
		const noncopyable& operator=( const noncopyable& );
	};
}

typedef noncopyable_::noncopyable noncopyable;

} // namespace huluzy

#endif  // __HULUZY_COMMON_NONCOPYABLE_H__
