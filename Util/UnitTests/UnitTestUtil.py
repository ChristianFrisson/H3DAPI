

def screenshot(start_time = None, run_time = None):
  def _screenshot(func):
    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time

    if (not (run_time is None) and (not hasattr(func, 'run_time') or (func.run_time < run_time))):
      func.run_time = run_time

    func.validation.append({'type': 'screenshot', 'name' : func.__name__})
    return func
  return _screenshot


def performance(start_time = None, run_time = None):
  def _performance(func):
    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time

    if (not (run_time is None) and (not hasattr(func, 'run_time') or (func.run_time < run_time))):
      func.run_time = run_time

    func.validation.append({'type': 'performance', 'name' : func.__name__})
    return func
  return _performance


def console(start_time = None):
  def _console(func):
    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time

    func.validation.append({'type': 'console', 'name' : func.__name__})
    return func
  return _console


def custom(start_time = None):
  def _custom(func):
    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time


    func.validation.append({'type': 'custom', 'name': func.__name__})
    return func
  return _custom

