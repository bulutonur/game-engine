#ifndef SIGNAL_H
#define SIGNAL_H

// Signal is a sigslot implementation
// It is used for delegates.

#include <stdint.h>
#include <functional>
#include <vector>

namespace Lib
{
	/**
	 * @brief ProtoSignal is the template implementation for callback list.
	 *
	 * @tparam typename
	 * @tparam typename
	 */
	template<typename, typename> class ProtoSignal;   // undefined

	/**
	 * @brief CollectorInvocation invokes signal handlers differently depending on return type.
	 *
	 * @tparam ypename
	 * @tparam typename
	 */
	template<typename, typename> struct CollectorInvocation;

	/**
	 * @brief CollectorLast returns the result of the last signal handler from a signal emission.
	 *
	 * @tparam Result
	 */
	template<typename Result>
	struct CollectorLast {
		typedef Result CollectorResult;
		explicit  CollectorLast() : last_()
		{

		}

		inline bool operator()(Result r)
		{
			last_ = r;
			return true;
		}

		CollectorResult result()
		{
			return last_;
		}

	private:
		Result last_;
	};

	/**
	 * @brief CollectorDefault implements the default signal handler collection behaviour.
	 *
	 * @tparam Result
	 */
	template<typename Result>
	struct CollectorDefault : CollectorLast<Result>
	{

	};

	/**
	 * @brief CollectorDefault specialisation for signals with void return type.
	 *
	 * @tparam
	 */
	template<>
	struct CollectorDefault<void>
	{
		typedef void CollectorResult;
		void                  result()
		{

		}

		inline bool operator() (void)
		{
			return true;
		}
	};

	/**
	 *  @brief CollectorInvocation specialisation for regular signals.
	 *
	 * @tparam Collector
	 * @tparam R
	 * @tparam Args
	 */
	template<class Collector, class R, class... Args>
	struct CollectorInvocation<Collector, R(Args...)>
	{
		inline bool invoke(Collector& collector, const std::function<R(Args...)>& cbf, Args... args)
		{
			return collector(cbf(args...));
		}
	};

	/**
	 * @brief CollectorInvocation specialisation for signals with void return type.
	 *
	 * @tparam Collector
	 * @tparam Args
	 */
	template<class Collector, class... Args>
	struct CollectorInvocation<Collector, void(Args...)>
	{
		inline bool invoke(Collector& collector, const std::function<void(Args...)>& cbf, Args... args)
		{
			cbf(args...);
			return collector();
		}
	};


	/**
	 * @brief ProtoSignal template specialised for the callback signature and collector.
	 *
	 * @tparam Collector
	 * @tparam R
	 * @tparam Args
	 */
	template<class Collector, class R, class... Args>
	class ProtoSignal<R(Args...), Collector> : private CollectorInvocation<Collector, R(Args...)>
	{
	protected:
		typedef std::function<R(Args...)> CbFunction;
		typedef typename CbFunction::result_type Result;
		typedef typename Collector::CollectorResult CollectorResult;
	private:

		/**
		 * @brief SignalLink implements a doubly-linked ring with ref-counted nodes containing the signal handlers.
		 */
		struct SignalLink
		{
			SignalLink* next, * prev;
			CbFunction  function;
			int         ref_count;

			explicit    SignalLink(const CbFunction& cbf) : next(nullptr), prev(nullptr), function(cbf), ref_count(1)
			{

			}

			/*dtor*/
			~SignalLink()
			{
				ASSERT(ref_count == 0);
			}
			void incref()
			{
				ref_count += 1;
				ASSERT(ref_count > 0);
			}

			void decref()
			{
				ref_count -= 1;
				if (!ref_count)
				{
					delete this;
				}
				else
				{
					ASSERT(ref_count > 0);
				}
			}

			void unlink()
			{
				function = nullptr;
				if (next)
				{
					next->prev = prev;
				}
				if (prev)
				{
					prev->next = next;
				}
				decref();
				// leave intact ->next, ->prev for stale iterators
			}

			size_t add_before(const CbFunction& cb)
			{
				SignalLink* link = new SignalLink(cb);
				link->prev = prev; // link to last
				link->next = this;
				prev->next = link; // link from last
				prev = link;
				static_assert (sizeof(link) == sizeof(size_t), "sizeof size_t");
				return size_t(link);
			}

			bool deactivate(const CbFunction& cbf)
			{
				if (cbf == function)
				{
					function = nullptr;      // deactivate static head
					return true;
				}
				for (SignalLink* link = this->next ? this->next : this; link != this; link = link->next)
				{
					if (cbf == link->function)
					{
						link->unlink();     // deactivate and unlink sibling
						return true;
					}
				}
				return false;
			}

			bool remove_sibling(size_t id)
			{
				for (SignalLink* link = this->next ? this->next : this; link != this; link = link->next)
				{
					if (id == size_t(link))
					{
						link->unlink();     // deactivate and unlink sibling
						return true;
					}
				}
				return false;
			}
		};

		/**
		 * @brief linked ring of callback nodes
		 */
		SignalLink* callback_ring_;

		/**
		 * @brief copy-ctor
		 */
		ProtoSignal(const ProtoSignal&) = delete;
		ProtoSignal& operator=   (const ProtoSignal&) = delete;

		void ensure_ring()
		{
			if (!callback_ring_)
			{
				callback_ring_ = new SignalLink(CbFunction()); // ref_count = 1
				callback_ring_->incref(); // ref_count = 2, head of ring, can be deactivated but not removed
				callback_ring_->next = callback_ring_; // ring head initialization
				callback_ring_->prev = callback_ring_; // ring tail initialization
			}
		}
	public:
		/**
		 * @brief ProtoSignal constructor, connects default callback if non-nullptr.
		 * @param method
		 */
		ProtoSignal(const CbFunction& method) :
			callback_ring_(nullptr)
		{
			if (method != nullptr)
			{
				ensure_ring();
				callback_ring_->function = method;
			}
		}

		/**
		 * @brief ProtoSignal destructor releases all resources associated with this signal.
		 */
		~ProtoSignal()
		{
			if (callback_ring_)
			{
				while (callback_ring_->next != callback_ring_)
				{
					callback_ring_->next->unlink();
				}
				ASSERT(callback_ring_->ref_count >= 2);
				callback_ring_->decref();
				callback_ring_->decref();
			}
		}

		/**
		 * @brief Operator to add a new function or lambda as signal handler, returns a handler connection ID.
		 *
		 * @param cb
		 * @return size_t
		 */
		size_t connect(const CbFunction& cb)
		{
			ensure_ring();
			return callback_ring_->add_before(cb);
		}

		/**
		 * @brief Operator to remove a signal handler through it connection ID, returns if a handler was removed.
		 *
		 * @param connection
		 * @return true
		 * @return false
		 */
		bool disconnect(size_t connection)
		{
			return callback_ring_ ? callback_ring_->remove_sibling(connection) : false;
		}

		/**
		 * @brief Emit a signal, i.e. invoke all its callbacks and collect return types with the Collector.
		 *
		 * @param args
		 * @return CollectorResult
		 */
		CollectorResult emit(Args... args)
		{
			Collector collector;
			if (!callback_ring_)
			{
				return collector.result();
			}

			SignalLink* link = callback_ring_;
			link->incref();
			do
			{
				if (link->function != nullptr)
				{
					const bool continue_emission = this->invoke(collector, link->function, args...);
					if (!continue_emission)
					{
						break;
					}
				}
				SignalLink* old = link;
				link = old->next;
				link->incref();
				old->decref();
			} while (link != callback_ring_);
			link->decref();
			return collector.result();
		}

		/**
		 * @brief Number of connected slots.
		 *
		 * @return int
		 */
		int size()
		{
			int size = 0;
			SignalLink* link = callback_ring_;
			link->incref();
			do
			{
				if (link->function != 0)
				{
					size++;
				}
				SignalLink* old = link;
				link = old->next;
				link->incref();
				old->decref();
			} while (link != callback_ring_);
			return size;
		}
	};

} // Lib
// namespace Simple

/**
 * @brief Signal is a template type providing an interface for arbitrary callback lists.
 * A signal type needs to be declared with the function signature of its callbacks,
 * and optionally a return result collector class type.
 * Signal callbacks can be added with operator+= to a signal and removed with operator-=, using
 * a callback connection ID return by operator+= as argument.
 * The callbacks of a signal are invoked with the emit() method and arguments according to the signature.
 * The result returned by emit() depends on the signal collector class. By default, the result of
 * the last callback is returned from emit(). Collectors can be implemented to accumulate callback
 * results or to halt a running emissions in correspondance to callback results.
 * The signal implementation is safe against recursion, so callbacks may be removed and
 * added during a signal emission and recursive emit() calls are also safe.
 * The overhead of an unused signal is intentionally kept very low, around the size of a single pointer.
 * Note that the Signal template types is non-copyable.
 *
 * @tparam SignalSignature
 * @tparam Lib::CollectorDefault<typename std::function<SignalSignature>::result_type>
 */
template <typename SignalSignature, class Collector = Lib::CollectorDefault<typename std::function<SignalSignature>::result_type> >
struct Signal /*final*/ :
	Lib::ProtoSignal<SignalSignature, Collector>
{
	typedef Lib::ProtoSignal<SignalSignature, Collector> ProtoSignal;
	typedef typename ProtoSignal::CbFunction             CbFunction;
	/// Signal constructor, supports a default callback as argument.
	Signal(const CbFunction& method = CbFunction()) : ProtoSignal(method) {}
};


/**
 * @brief This function creates a std::function by binding @a object to the member function pointer @a method.
 *
 * @tparam Instance
 * @tparam Class
 * @tparam R
 * @tparam Args
 * @param object
 * @param method
 * @return std::function<R(Args...)>
 */
template<class Instance, class Class, class R, class... Args> std::function<R(Args...)>
slot(Instance& object, R(Class::* method) (Args...))
{
	return [&object, method](Args... args) { return (object.*method) (args...); };
}

/**
 * @brief This function creates a std::function by binding @a object to the member function pointer @a method.
 *
 * @tparam Class
 * @tparam R
 * @tparam Args
 * @param object
 * @param method
 * @return std::function<R(Args...)>
 */
template<class Class, class R, class... Args> std::function<R(Args...)>
slot(Class* object, R(Class::* method) (Args...))
{
	return [object, method](Args... args) { return (object->*method) (args...); };
}

/**
 * @brief Keep signal emissions going while all handlers return !0 (true).
 *
 * @tparam Result
 */
template<typename Result>
struct CollectorUntil0
{
	typedef Result CollectorResult;
	explicit CollectorUntil0() : result_()
	{

	}
	const CollectorResult& result()
	{
		return result_;
	}
	inline bool operator() (Result r)
	{
		result_ = r;
		return result_ ? true : false;
	}
private:
	CollectorResult result_;
};

/**
 * @brief Keep signal emissions going while all handlers return 0 (false).
 *
 * @tparam Result
 */
template<typename Result>
struct CollectorWhile0
{
	typedef Result CollectorResult;
	explicit                      CollectorWhile0() : result_() {}
	const CollectorResult& result()
	{
		return result_;
	}

	inline bool operator() (Result r)
	{
		result_ = r;
		return result_ ? false : true;
	}
private:
	CollectorResult result_;
};

/**
 * @brief CollectorVector returns the result of the all signal handlers from a signal emission in a std::vector.
 *
 * @tparam Result
 */
template<typename Result>
struct CollectorVector
{
	typedef std::vector<Result> CollectorResult;
	const CollectorResult& result()
	{
		return result_;
	}

	inline bool operator() (Result r)
	{
		result_.push_back(r);
		return true;
	}

private:
	CollectorResult result_;
};

#endif