import logging
from logging import CRITICAL, DEBUG, ERROR, INFO, NOTSET, WARN, WARNING

from logs.colors import Fore as ForegroundColors

DEFAULT_FORMAT = '%(asctime)s.%(msecs)03d [%(levelname)s]\t- %(module)s:%(lineno)d %(message)s'
DEFAULT_DATE_FORMAT = '%I:%M:%S'
DEFAULT_COLORS = {
    DEBUG: ForegroundColors.CYAN,
    INFO: ForegroundColors.GREEN,
    WARNING: ForegroundColors.YELLOW,
    ERROR: ForegroundColors.RED,
    CRITICAL: ForegroundColors.RED
}

logger = None
formatter = None

_loglevel = DEBUG
_logfile = None
_formatter = None

def setup_logger(name=__name__, logfile=None, level=DEBUG, formatter=None, fileLogLevel=DEBUG):
    _logger = logging.getLogger(__name__)
    _logger.setLevel(level)

    _formatter = set_formatter(formatter)

    __stream_handler = logging.StreamHandler()
    __stream_handler.setFormatter(_formatter)
    __stream_handler.setLevel(level)
    _logger.addHandler(__stream_handler)

    return _logger, _formatter

def set_formatter(formatter=None):
    if formatter:
        _formatter = logging.Formatter(formatter)
    else:
        _formatter = logging.Formatter('%(asctime)s.%(msecs)03d [%(levelname)-8s] %(module)s:%(lineno)d >> %(message)s',
                                        datefmt='%I:%M:%S')
    return _formatter

def set_file_handler(logger, logfile, fileLogLevel=DEBUG):
    __file_handler = logging.FileHandler(logfile, mode='w')
    __file_handler.setFormatter(formatter)
    __file_handler.setLevel(fileLogLevel)
    logger.addHandler(__file_handler)

def reset_default_logger():
    global logger
    global formatter
    global _loglevel
    global _logfile
    global _formatter

    _loglevel = DEBUG
    _logfile = None
    _formatter = None

    if logger:
        for handler in list(logger.handlers):
            logger.removeHandler(handler)

    logger, formatter = setup_logger(name='logs_defalut', logfile=_logfile, level=_loglevel, formatter=_formatter)

reset_default_logger()
