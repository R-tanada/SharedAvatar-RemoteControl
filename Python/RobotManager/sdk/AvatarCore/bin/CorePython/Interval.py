# This Python file uses the following encoding: utf-8
from threading import Event, Thread


def set_interval(func, interval, *args, **kwargs):
    """JS set_interval implementation, runs the function with given interval
       and arguments and returns the set status

    Example:
        Usage example

    Note:
        Specific notes

    Args:
        func (object): Method or function that is goint to run in interval
        interval (float): Interval value in milliseconds
        args (tuple): Arguments that are passed to the function
        kwargs (tuple): Keyword arguments that are passed to the function
    """
    stopped = Event()

    def loop():
        while not stopped.wait(int(interval)*0.001):
            func(args=args, kwargs=kwargs)
    Thread(target=loop).start()
    return stopped
