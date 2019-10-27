# LSM
Log Structure Merge tree

This project was more a proof of concept to build the 4 classes:
set, multiset, map and multimap in once.

The implementation is missing many operations, like iteration or erase (among others).
But it was sufficient for the use I had.

This kind of data structure is interesting when you need to perform many writes.
It has some nice IO complexity with a correct behaviour for reads.

The whole point of this data structure is the notion of batch which does not work
as well in its big brother "B+-tree" / buffer tree.
