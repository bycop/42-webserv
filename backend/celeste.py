#!/usr/bin/python3
from art import *
import base

Art = text2art("Warning\nserver", "rand")

# str = art("random")
# str += tprint("Warning Server")
base.print_base("<pre>" + Art + "</pre>")
