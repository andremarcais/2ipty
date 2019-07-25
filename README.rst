2ipty
#####

``2ipty`` is a pseudo terminal that reads a fifo and your terminal for input.
It can be useful for terminal applications that you may want to control with some keybinding.
I use this with `mpsyt <https://github.com/mps-youtube/mps-youtube>`_, which I use as a music player.
Sometimes I want to play/pause without looking for the terminal window with ``mpsyt``.
To do this, I bound the ``XF86AudioPlay`` key to ``echo -ne ' ' > /tmp/my-mpsyt.fifo`` (in my i3config).
Now, when I press ``XF86AudioPlay`` anywhere or the space bar in the player, the music is played or paused.

Ussage
------

.. code:: text

    2ipty <cmd> <input path>

Example
-------

tty1:

.. code:: bash

    $ 2ipty python /tmp/toto.fifo
    >>> print("This is typed into tty1")
    This is typed into tty1
    >>> print("This is from /tmp/toto.fifo")
    This is from /tmp/toto.fifo
    >>> 

tty2:

.. code:: bash

    $ echo 'print("This is from /tmp/toto.fifo")' > /tmp/toto.fifo
