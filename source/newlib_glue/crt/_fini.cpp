/*
    Rel mods handle ctors and dtors through _prolog and _epilog rather than the
    traditional methods

    __fini would usually come from the crt code, which we don't link in rel
    mods, so we define our own which is a simple forwarder to _epilog
*/

extern "C" {

extern void _epilog();

void __fini()
{
    _epilog();
}

}
