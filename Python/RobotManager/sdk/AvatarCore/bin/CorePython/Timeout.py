# This Python file uses the following encoding: utf-8
from threading import Timer


def set_timeout(func, ms, *args, **kwargs):
    """JS set_timeout implementation. Runs the given function or method after
       times out

    Example:
        Usage example

    Note:
        Specific notes

    Args:
        func (object): Function or method that is going to run after timeout
        ms (float): Timeout value in milliseconds
        args (tuple): Arguments that are passed to the function
        kwargs (tuple): Keyword arguments that are passed to the function
    """
    t = Timer(ms / 1000., func, args=args, kwargs=kwargs)
    t.start()
    return t
