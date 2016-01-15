

def screenshot(start_time):
  def _screenshot(func):
    if not hasattr(func, 'validation'):
      func.validation = []
    if ((start_time) and not hasattr(func, 'start_time') or (func.start_time < start_time)):
      func.start_time = start_time
    func.validation.append({'type': 'screenshot', 'name' : func.__name__})
    return func
  return _screenshot


def performance(start_time, run_time):
  def _performance(func):
    if not hasattr(func, 'validation'):
      func.validation = []
    if ((start_time) and not hasattr(func, 'start_time') or (func.start_time < start_time)):
      func.start_time = start_time
    if ((run_time) and not hasattr(func, 'run_time') or (func.run_time < run_time)):
      func.run_time = run_time
    func.validation.append({'type': 'performance', 'name' : func.__name__})
    return func
  return _performance


def console(start_time):
  def _console(func):
    if not hasattr(func, 'validation'):
      func.validation = []
    if ((start_time) and not hasattr(func, 'start_time') or (func.start_time < start_time)):
      func.start_time = start_time
    func.validation.append({'type': 'console', 'name' : func.__name__})
    return func
  return _console


def custom(start_time):
  def _custom(func):
    if not hasattr(func, 'validation'):
      func.validation = []
    if ((start_time) and not hasattr(func, 'start_time') or (func.start_time < start_time)):
      func.start_time = start_time
    func.validation.append({'type': 'custom', 'name': func.__name__})
    return func
  return _custom