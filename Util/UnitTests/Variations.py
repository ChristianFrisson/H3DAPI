
import re

class Option ( object ): 
  """ Specifies the value of a field that is identified by its name and its container node. """
  def __init__ ( self, node_type, field_name, field_value, container_node = "Scene", add_if_missing = False ):
    self.node_type= node_type
    self.field_name= field_name
    self.field_value= field_value
    self.container_node = container_node
    self.add_if_missing = add_if_missing

    
class Variation ( object ):
  """ Specifies a set of Options to run a test with and provides functionality for inserting those into a given x3d string. """

  def __init__ ( self, name= "Default", insertion_string = "" ):
    self.name= name
    self.options= []
    self.global_insertion_string = insertion_string
    self.global_insertion_string_failed = False
    
  def parse ( self, input ):
    """ Parses the input x3d string and returns a new x3d string where this Variation's Options have been inserted into the appropriate nodes. """
    replace_with= ""
    output= input
    reg_use = None
    reg_field = None
    self.global_insertion_string_failed = False
    
    def replace_callback(r):
      if r.group(1):
        if reg_use.search( r.group(0) ):
          # If USE found, then return the pattern as is.
          return r.group(0)
        field_match = reg_field.search( r.group(0) )
        if field_match:
          # If field is found, then simply return a string in which the field is changed.
          return r.group(0)[0:field_match.start()] + replace_with + r.group(0)[field_match.end():]
        else:
          # If field is not found, add it.
          return "<%s %s%s" % (r.group(1),r.group(2),replace_with)
      else:
        return r.group(0)
      
    if self.global_insertion_string != "":
      reg = re.compile( r'<Scene>' )
      output_after = reg.sub('<Scene>\n' + self.global_insertion_string, output)
      if output_after == output:
        self.global_insertion_string_failed = True
      else:
        output = output_after
          
    for o in self.options:
      for node_type in o.node_type:
        reg = re.compile(r'<('+node_type+r')\s([^>/]*)', re.DOTALL)
        reg_use = re.compile(r'\s+USE\s*=\s*')
        reg_field = re.compile(o.field_name+r'\s*=\s*(?:\'[^\']*\'|"[^"]*")' )
        if o.field_name == "":
          replace_with = ""
        else:
          replace_with= " %s=\"%s\"" % (o.field_name, o.field_value)
        output = reg.sub(replace_callback, output)
        
        if o.add_if_missing:
          reg = re.compile( r'<'+node_type+r'[^>]*>')
          if reg.search( output ) == None:
            reg = re.compile( r'(<'+o.container_node+r'[^>]*>\s*)' )
            replace_string = r'\1<' + node_type + ' '
            if o.field_name != "":
              replace_string = replace_string + o.field_name + '=\'' + o.field_value + '\' '
            replace_string = replace_string + '>\n'
            replace_string = replace_string + '</' + node_type + '>\n'
            output = reg.sub( replace_string, output )
    return output    

 