
Where to find the schematic and users manual (and ARM ARM) is described
in the top level README file.

This sample is derived from blinker04, 12MHz external speed and pll
bypassed to give the processor 12MHz.

Although I have clearly demonstrated that you dont need to use interrupts
to do timing or blink leds or anything like that, the added complexity
and system engineering to use interrupts is not trivial, dont use them
unless you have a really good reason.  The alternative is do everything
with interrupts and nothing in the foreground task.

The systick timer looks to be clean and easy, apparently designed just
for periodotic interrupts.

Need to do some research in the ARM ARM.

This is not like the traditional ARM, more and more this core is like
other processors than an ARM.  For example on an ARM core there are
banked registers, when you have an interrupt, you have a certain
number of interrupt mode registers, for example there is an interrupt
stack pointer, which is different from other stack pointers so that
you can isolate the stacks.  What the cortex-m3 is doing here
is not only does it rely on the normal stack to preserve the state
of the machine.  It preserves the state of the machine for you by
pushing a number of registers on the stack.  Which resgisters?  Well
the ones defined in the ABI as registers that are somewhat volitile
within a function.  Also there is a lot of confusing verbage about
different modes and returning from the exception depending on the mode,
etc.  Basically though they put the value you need to use to return
from the interrupt in the link register so just like any other
function you use a normal function return (pop pc or bx lr or something
like that).   So what this means is that if you are using the right
compiler you can have the vector table put the address of the handler
in C, and not have to use any assembly.  Pretty slick if it works.

So this code blinks two leds, it uses timer0 to blink one of the leds
and uses the interrupt service routine based on the systick interrupt
to blink the other led at a different rate.  the two are ultimately
driven by the same clocks and use comperable timers, basically the
fast led is 6 times faster than the slow.  When the slow led changes
it will also appear that its when the fast is having one of its
state changes.
