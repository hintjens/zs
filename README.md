
<A name="toc1-2" title="This an Experiment... ZeroScript" />
# This an Experiment... ZeroScript

[![Build Status](https://travis-ci.org/zeromq/czmq.png?branch=master)](https://travis-ci.org/zeromq/czmq)

<A name="toc2-7" title="Contents" />
## Contents


**<a href="#toc2-18">This an Experiment</a>**

**<a href="#toc3-51">Inspirations</a>**

**<a href="#toc3-86">Irritations</a>**

**<a href="#toc3-143">Experimental Notes</a>**

**<a href="#toc3-181">Words, Phrases, and Sentences</a>**

**<a href="#toc3-312">Defining a Function</a>**

**<a href="#toc3-329">Irritating and Iterating</a>**

**<a href="#toc3-345">The Code</a>**

**<a href="#toc3-358">The Virtual Machine</a>**

**<a href="#toc3-373">Extensibility</a>**

**<a href="#toc3-406">Code Generation</a>**

**<a href="#toc3-414">The Shell</a>**

**<a href="#toc3-421">Arguments</a>**

**<a href="#toc3-430">Other Goals</a>**

**<a href="#toc2-449">Design Notes</a>**

**<a href="#toc2-457">Bibliography</a>**

**<a href="#toc2-466">Technicalities</a>**

**<a href="#toc3-469">Ownership and License</a>**

**<a href="#toc3-480">Building and Installing</a>**

**<a href="#toc3-513">This Document</a>**

Seriously, this is renewing my hope in technology. Thanks @hintjens -- Jason J. Gullickson ‏@jasonbot2000

Some very interesting thoughts ... esp. for #IoT applications -- Till Hänisch ‏@TillHaenisch

Is this a joke or are you serious? -- flyingfisch (entirely out of context)

<A name="toc2-18" title="This an Experiment" />
## This an Experiment

I want a language that makes it trivially easy to write very large scale distributed apps. So easy that I can teach this to my kids and watch them program a thousand devices. My son is four. He uses a computer every day, and plays with his sister's discarded smartphone. Every 18 months, he'll be playing with twice as many devices. By eighteen he'll have a thousand on his desk. He's my target.

Wouldn't it be nice if we could send the code to the data? I mean, this is Web 2.0. I believe it's why JavaScript is such a popular language. Solves the right problem. To deploy JavaScript you push it to a web server. You don't need a compiler, linker, packages, containers, downloads, installers. The tools of our industry, so wrong in so many ways. I like C, yet am keenly aware of the layers of friction between me and my goal of making machines do fun stuff.

So JavaScript is successful despite the actual language, because you can ship the code to a hundred million boxes with no more effort than shipping it to a single one. This is incredibly powerful. Failure costs nothing, and success scales without limit.

Let me observe that the Web is just a poor messaging system. Replace that old clunky HTTP with something like ZeroMQ, and you remove even the remaining cost to scaling. Suddenly you don't need massive servers at the heart of your networks. We can move from Soviet-style central planning to a real market. "You want some code? I got some code."

There's a second reason for JavaScript's emerging domination. That is, we share source code. When you receive JavaScript, you get source code you can read, modify, and reuse. It may not be entirely legal, and people try to prevent this, yet it's the dominant culture. If I get to use your CPU cycles then you can remix my code.

Remixability isn't an added feature. It's a cultural vitamin. Without it, culture grows stunted and crooked, and dies way too young. As does technology. Our industry is built on the corpses of projects too proud or stupid or greedy to take their vitamins. All my closed source work, ever, died. Much open source also dies. I said "vitamin", not "complete diet".

Sending source code around the network, and forced remixability go nicely together. I'll cover the legal aspects later. Let's look at sending source code.

Sending source code -- rather than bytecode or worse, machine code -- means that code becomes a valid question, "can you do this?" Let's say I send this to every device in the house:

    temperature name location

A handful will reply, and most will just ignore the request. It's like shouting "who wants beer?" in a cafe in Seoul. You will pick out the English speakers, and the rest of the house will pretend you're not there.

The real world is fuzzy. It's approximate. And it's incredibly lazy. I don't know exactly how many mobile phones I own. Three, maybe. Five, if I count my kids' phones. How can I possibly manage the apps they run? The "personal computer" does not scale. The Web already smashed that paradigm in the last century. Kudos to Sun for recognizing this and pushing it so hard for so long. "The network is the operating system" was Sun's slogan.

My kids are also lazy, so this new language has to be simple. I don't mean trivial. It can be as deep as the ocean. The deeper the better, in fact. However it should start with a gentle paddle in toe-deep water.

We already covered "throwable" (shouting code at a crowd of devices and seeing what happens) and "remixable" (shouting source code, not some stripped form). It has to embrace fuzzy reality, where failure is valid data. I've learned that a successful open source community is a learning machine that needs a steady diet of small, fun failures. This is how I want to build distributed systems.

It must be "concurrent" by default. Obviously things happen all over the place, because that's how reality works. Every atom spins alone, every brain sees its own world. Why is our software  mostly glued to a fake view of a monolithic world? Get rid of that view, and there's nothing to learn.

Lastly, it should be fun to make stuff, share stuff. I want the process of using the language to flow through a learn-play-work-teach cycle. Again, it must happen from first contact. Here's how you make a brick. Here's what the brick can do. Try it! It's safe! Now you can use the brick.

<A name="toc3-51" title="Inspirations" />
### Inspirations

I remember the moment that computers turned fun, and I turned from being a failing CompSci student to top of the class. My mother bought me a VIC 20 and I started programming it. Then Commodore gave me a C-64 to play with, just because. I wrote games and sold them on cassette, paying my way through studies and beer. But BASIC ran too slowly, and assembler was too much work (and ran too fast, seriously). So for my thesis I wrote a language for making games.

My inspiration was the book, "Threaded Interpretive Languages", by R. G. Loeliger. This remains one of the best books ever written on our art. The book is very hard to find in paper. Luckily there's [a PDF available](http://sinclairql.speccy.org/archivo/docs/books/Threaded_interpretive_languages.pdf).

The C-64's 6502 CPU was a [work of minimalist art](http://e-tradition.net/bytes/6502/6502_instruction_set.html). It only has two general 8-bit registers, X and Y. It has a stack pointer and an accumulator. The instructions are: add, and, branch, compare, decrement, exor, increment, jump, load, shift, push, pop, rotate, store, and subtract. No multiply or divide.

On top of that I built a layer of assembly primitives, and then an assembler and an editor, and then games. Loeliger described how to build Forth, or something like it. Forth is what we should have been studying at university, instead of Pascal.

The thing about Forth, like Lisp, is that your language is the machine. No abstraction stands between your words and the behavior of the "computer". I put that into quotes because obviously you're talking to a virtual computer. Nonetheless, you write executable code. Put that into a read-evaluate-print-loop (REPL) and you get something mind-blowing and addictive.

Forth is also notable for other reasons. It has a tiny core syntax and almost no grammar. The bulk of the language is self-hosting. This means it runs well in very small environments. This is relevant in 2015 because the world is increasingly filled with tiny dirt-cheap computers begging to be programmed. Just not in Java, please.

When I write C I always try to recreate that REPL feeling. I make little scripts that compile, link, run, and print "OK" when the test passes. It works yet it's still slower and clumsier than I'd like, and my precious code still sits on the wrong side of a high, non-portable compile-link-deploy wall.

I like REPL shells that recreate my ideal learn-play-work-teach cycle. It makes programming fun (again). And when programming is fun, and easy, and addictive, then we can dream of a world where to use a device and to program it are the same. A world where we own our software just as we own our languages, our recipes, our poems, our songs.

There are a few other things I really enjoy, from a long career writing software in anger. Perhaps the most powerful is the Finite State Machine (FSM). It's hard to over-state how profitable FSMs have been for me, in taming complex problems and making software that just worked, and never crashed.

My first free software tool was [Libero](http://legacy.imatix.com/html/libero/), which I used in many projects and which had a loyal following. Its charm was that one wrote FSMs as simple text, which Libero turned into code. This was also my first code generator in C, and could produce FSMs in any language. We did C, Pascal, Java, and even PL/SQL and COBOL.

Libero has no bugs as far as I know, and the bits did not rot. However we got much better at code generation, so today I can build [a FSM code generator](https://github.com/hintjens/zs/blob/master/fsm_c.gsl) in a few hours, whereas Libero took me months of work.

The Libero-style state machine is a high level event-driven control language. It replaces iterations and conditionals. At its heart it says, "when I am in state S and I get event E, then do actions A, B, and C". The biggest downside with writing FSMs is that we need two languages; one for the control and one for the actions. I'd like to make this a single language.

I like events and especially, queues of events, which are pipes carrying messages and commands. We explored this in CZMQ 3.0, with actors. These turn out to work incredibly well in C, a language that has no native support for concurrency. We learned how to build very solid message-based APIs, between pieces in the same program.

I'll explore actors in more detail much later. For now, let me pin "pipes" on the wall, and remind you of the widely known and under-appreciated concurrent REPL environment called the Unix shell. When you connect commands with pipes, these run concurrently, the output from one becoming the input to the next. It is a simple and obvious model, and fits many of our criteria for distributed code. State sits in the messages, not the code. Each piece can run asynchronously.

So in my language, the default flow is output-to-input, pipes carrying messages and commands from one piece to the next. It's a starting point: I'm sure we'll need more sophisticated queues later.

Other inspirations are obvious: Erlang, Go, Rust, and Clojure. I've noticed from several ZeroMQ workshops that people using Clojure always seem to get their examples done fastest. I suspect it's the REPL again. Whatever, when someone can write ZeroMQ code faster than me, it's time for me to shift to newer tools. And that means moving away from C, at least for the 90% of cases that don't need a systems language.

<A name="toc3-86" title="Irritations" />
### Irritations

Now to stuff I dislike and want to avoid.

Both Forth and Lisp offer you their salaciously unfiltered virtual machines, and yet both could be simpler. I do not like reverse Polish notation. Stacks are fine data structures, yet they are not intuitive for humans. This hurts my brain, and I don't want to have to try to explain it to anyone:

    1 2 3 4 5 + + + +

Whereas my young daughter correctly and immediately understood this without any explanation:

    1 2 3 4 5 add

And then extrapolated to "you could also say average, or median!"

What should this do?

    1 2 3 4 5

Answer: print "1 2 3 4 5" and not "5 4 3 2 1".

Forth is too low on the abstraction level. Do I want to have to explain things like word sizes to my kids just so they can switch on the heating? Clearly not. We can't make throwable code if we care about bits and bytes. And we have C to deal with that part.

I never used Lisp, so my criticisms here are vague and opinionated. I dislike recursion except when it's the obvious algorithm, which means for navigating recursive data structures like trees. I'm not a great fan of trees, as nested structures tend to confuse people.

Long ago I learned most people can learn three levels of nesting: this one, the one above, and the one below. Recursion also reinforces the common fallacy that because things at different levels are self-similar, *they are the same*. Star systems may spin like atoms. However they are not the same thing. This is my data. This is the parent. Here are its children. Do the appropriate work, and climb up and down the tree as needed.

Lisp's parenthesis feel like they're in the wrong place:

    (command argument argument)

Which I'd rather write like:

    command (argument argument)

So no stacks, RPNs, or lists of lists of lists. No recursive descent parsers either. A language that needs recursion to parse it is too complex. Come to think of it, arrays and other Von Neumann artifacts annoy me too. Natural things come in sets, queues, clusters, crowds, and clouds.

I dislike error handling. Partly it's from laziness. More though, it's from experience. Oh, great, you got an error code from a library call! What do you do now? It's like getting a phone call from your takeaway pizza place telling you their oven is switched off. What do you do now? Wait, abort, or retry?

No, real code never gives errors. It either works or it dies grimly and with minimal noise. The tolerance of ambiguity causes the very worst crashes. So I want the Erlang approach, where code goes off and tries stuff, and either succeeds or kills itself.

Let me talk about bits and bytes. I love the C language but sweet lord I hate being confronted by computer word sizes and all precision-related junk. It an inevitable and yet always sad moment when I have to introduce precisions into domain-specific languages. Computer, it's a *number*, isn't that good enough for you? Apparently, not.

Tied to precision is the shoddy way we ask people to write numbers. We are in Era of Large Numbers yet we have to count digits like a Fortran programmer counting Hollerith strings. How do you write 10 trillion in your language of choice? Or 99%? Or 2^16-1? I'd like all these to be obvious and simple, to reduce errors and make life easier.

And then conditionals and iterations. Oh so your compiler can do Boolean logic? Wow, have a cookie. Now tell me why you've made "1" a special case? Using "if" statements in any depth is an anti-pattern, and using loops of any complexity is also an anti-pattern. As for choosing between a series of "if" statements, or a single "switch" statement, just to try to second-guess the compiler... this is close to insanity.

So control flow should stem from the natural models we use, not the capacity of our compilers and CPUs. This means, event-driven action ("when", not "if") and lazy infinite looping (do stuff never, once (more), or forever until). Perhaps a little state machining, below the water.

Do I have to say I hate shared state and am glad for the chance to start afresh without it? Objects do work, as a model for packaging functionality and data structure. I'm glad we got such structure into our C code, in the form of CLASS. That is wonderfully tidy and safe. However it's still clumsy, clumsy, clumsy. Any real application spends most of its time sending objects from one place to another.

Structure is fine. However I want to send data around the application, and never hold it anywhere. Messages = data, period. This gives us interesting queuing and scalability. If my sqrt function is too slow, I can start a dozen of them. A function looks the same whether it's a call/return in the same thread, a thread in the same process, a process on the same box, or another box in the same cloud. ZeroMQ taught us the value of such symmetry.

And when data is message, we can handle bad data simply by discarding it. Functions without state should never crash. Is this a late conversion to functional programming? Could be.

I'm also going to experiment with better text forms. Conventional strings don't work that well, leading to Python's """ and Perl's "OK, I give up, do whatever you like" solutions. I don't see why regular expressions, commands, keystrokes, or template code should have different syntaxes. They're all text. For now I'm using < and >, and will explore other ways to represent text.

<A name="toc3-143" title="Experimental Notes" />
### Experimental Notes

Most language designers use grammar like early web designers used fonts. The more the merrier, surely! After all, why did God give us such a rich toolkit for building languages, if we were not meant to use it?

Just as every new font creates a reason for the reader to stop reading. every new syntactic element is a reason to not use a given language. Less is more, and more is less.

The basic grammar of ZeroScript is therefore just a series of numbers, strings, and commands, in a structure that seems to make sense. I'm not convinced this design will work at all, it just feels nice for now.

The language looks like this (taken from the VM self test):

    sub: (<OK> <Guys> tally 2 assert)
    main: (
        123 1000000000 sum 1000000123 assert,
        <Hello,> <World> tally 2 assert,
        add (123 456) 579 assert,
        add (123 tally (1 2 3)) 126 assert,
        year year tally 2 assert
    )
    sub sub main

* Strings are enclosed in < and > rather than " and " which are unpleasant to parse properly.
* Functions are postfix, or prefix if you use ( and ) to enclose their arguments.
* Words are grouped into sentences and phrases, which I'll explain in a second.
* The rest should be obvious at first reading, that is the point.

It's a softly functional language. Functions have no state, and there are no variables or assignments. A function can produce a constant value:

    > greeting: (<Hello, fellow humans!>)
    > greeting
    <Hello, fellow humans!>

Our grammar thus has just a few syntactic elements:

    function: ( something )
    function ( something )
    something, something, something.

<A name="toc3-181" title="Words, Phrases, and Sentences" />
### Words, Phrases, and Sentences

Forgive me for this. This is an experiment in making the language easier and more obvious. It seems to work and is pleasant to use.

These should all be obvious:

    > 1 2 3
    1 2 3
    > 1 2 3 add
    6
    > 1 2 3 tally, 1 1 add
    3 2
    > 1 2 3 tally, 1 1 add, subtract
    1

I'll come to adding aliases like "+" and "-" later. How does that final "subtract" know what to do? Let me make a more delicate example of my target syntax:

    > 1 day 2 weeks add
    1296000

Which shows the result in seconds. Or,

    > 1 day 2 weeks add 1 day divide
    15

Note how the phrase is built up from left to right. It is simpler to use parentheses:

    > divide (add (1 day 2 weeks) 1 day)

However when we write this, we're always editing the *start* of the phrase rather than adding to it at the end. Also, parenthesis... easy for parsers, harder for humans.

I can break this phrase into smaller phrases:

    > 1 day 2 weeks add, 1 day, divide
    15

There seem to be three kinds of function:

* Functions that take no arguments and produce some magic value. These are key to measuring the real world: time, temperature, free disk space, etc. It makes no sense to write these. One does not modify the current temperature. Side-effects seem honest, e.g. switching on the heating or cooling should eventually change the measurable temperature. I use the standard term "nullary" for these functions.
* Functions that take a single argument, and which can be convinced to work on a set or list of values. For example, the "day" and "weeks" functions are multipliers (returning the number of seconds in a day and week respectively). I call these "modest" functions.
* Functions that take a list of arguments. These functions should be able to work on lists of zero, one, two, or a thousand values. I call these "greedy" functions.
* Functions that apply one value to a list of values. For example, "multiply these numbers by ten". I call these "array" functions.

When we define atomics, we tell the virtual machine what type of function we're making (nullary, modest, greedy, or array). When we construct functions out of others, the new function takes the type of the first word in its body.

The input to a non-nullary function forms a FIFO list. The output of one function has to go magically to the next. I wanted to avoid having to write special symbols (like the UNIX pipe character) to make this magic happen.

It turns out we can make this work pretty well using a single special symbol (comma) to provide hints to the compiler. The comma breaks the code into "phrases". Each phrase produces some output, which accumulates as we work through the phrases.

The commas work well, so I'm using a period to end a sentence. What this does is print the previous phrases' output, and start afresh.

Before we call a function, the VM prepares an input pipe based on the type of the function and where we use it:

* A modest function operates on the previous value in the current phrase, if there's one: "1 k". If we use a modest function at the start of a phrase, then it operates on the whole previous phrase: "1 2 3, k". This is like using parentheses: "k (1 2 3)".
* A greedy function operates on the previous values in the current phrase, if there are any: "1 2 3 add". If we use a greedy function at the start of a phrase, it operators on the whole sentence so far: "1 2 3, 4 5 6, add".
* An array function applies one value (the last value) to a phrase. Internally the last value is passed first, so the function can use it as an operand: "1 2 3, 2 x". The comma can help readability though isn't necessary.

This language style is sometimes called *concatenative*. However most such languages force the user to learn reverse-Polish notation and the mechanics of a stack, neither of which are intuitive. FIFO pipes seem more intuitive and more fitting for a language that aims to stretch itself over multiple threads, cores, boxes, and clouds.

I'm trying to avoid forced-binary operations, as two seems an arbitrary special case. More importantly, infix notation doesn't work well with the concatenative style. Hence the array functions. Here is how array functions work:

    > 1 2 3, 2 x
    2 4 6
    > 22 7 /
    3.14286
    > 21 22 23, 7 /
    3 3.14286 3.28571

Once we have this working, we can start to make numbers fun to write:

    > 64 Gi
    68719476736
    > 2 Pi
    2251799813685248
    > 32 Ki
    32768
    > Gi (16 32 64)
    17179869184 34359738368 68719476736

Here the scaling function works on a list rather than a single value. I like the first form because it reduces the need for parenthesis. The second form is less surprising to some people, and lets us nest functions.

Type the name of a scaling function to see what value it uses:

    > Gi
    1073741824
    > Z
    1e+21

There are also the SI fractional scaling functions (d, c, m, u, n, p, f, a, z and y):

    > 15 u
    1.5e-05
    > 1 m
    0.001
    > 1 a
    1e-18

Scaling functions let you calculate in natural units. These functions multiply their input (scale up). There are corresponding scale-down functions which start with "/":

    > 150 M /year
    4.75647

The year, month, day, week, msec functions scale up or down to a number of seconds:

    > 150 M /year msec
    0.00475647

Which is a nice simple way of calculating "If I have to handle 150 million requests a year, how many is that per millisecond?"

There's a function 'help' that prints all available functions:

    > help
    check help sum product count mean min max assert whole plus + minus
    - times * x divide / Ki /Ki Mi /Mi Gi /Gi Ti /Ti Pi /Pi Ei /Ei da
    /da h /h k /k M /M G /G T /T P /P E /E Z /Z Y /Y d /d c /c m /m u
    /u n /n p /p f /f a /a z /z y /y minutes minute /minutes /minute
    hours hour /hours /hour days day /days /day weeks week /weeks /week
    years year /years /year msecs msec /msecs /msec

Numbers are either whole numbers or real numbers. Wholes get coerced into real automatically as needed. To get the closest whole for a given real, use the 'whole' function.

For convenience, numbers can end with '%' indicating they're a percentage (this just divides the value by 100). Real numbers can be specified in scientific notation. So:

    > 23%
    0.23
    > 23e-4
    0.0023

You can write comments using '#' until the end of the line. There are no multiline comments. Longer comments should perhaps be defined as functions, e.g. license, author, and so on.

<A name="toc3-312" title="Defining a Function" />
### Defining a Function

To define a new function (or redefine any existing function), use this syntax:

    name: ( body )

Where the body can be any valid sentence, including empty. The body can stretch over many lines, as long as the ": (" comes together on the same line as the function name. Here's how to alias an existing function:

    run_my_selftests: (check)

Here are some other examples

    > K (1000 *)
    > K (1 2 3)
    1000 2000 3000

<A name="toc3-329" title="Irritating and Iterating" />
### Irritating and Iterating

Here is how we repeat some actions:

    > hi-hole: ( 3 times { <well> } )
    > hi-hole
    well well well

Or, to get a sequence of numbers:

    > count (10) { }
    1 2 3 4 5 6 7 8 9 10
    > count (10 0 2) { }
    0 2 4 6 8 10 12 14 16 18

<A name="toc3-345" title="The Code" />
### The Code

The code has these pieces:

* A lexer (zs_lex) that breaks input into tokens. This should become a language atomic at some stage, pushing input tokens to a pipe. It's based on a state machine.
* A pipe class (zs_pipe) that holds numbers and strings. This is not meant to be fast; it just shows the idea.
* A virtual machine class (zs_vm) that compiles and executes bytecode. It supports a basic syntax that I'll explain in a second.
* A REPL class (zs_repl) that connects the lexer to the VM, based on a state machine.
* A main program (zs) that acts as a shell.

The fsm_c.gsl script builds the state machines, which are XML models that drive GSL code generation.

<A name="toc3-358" title="The Virtual Machine" />
### The Virtual Machine

I finally settled on a bytecode threaded interpreter. The 'threading' part refers to the way the code runs together, not the concurrency. However the play on words may be fun later. A metal direct threaded interpreter literally jumps to primitive functions, which jump back to the interpreter, so your application consists of 90% hand-written assembler and 10% glue. It's elegant. It doesn't work in ANSI C, though gcc has a hack "goto anywhere" trick one could use. One is not going to, at this stage.

So we use a token threaded bytecode interpreter. That is, a byte is an instruction. The inner interpreter steps through opcodes one by one, until and if the machine finishes. Where opcodes take parameters, these come just after, byte by byte.

The fastest way to decode such opcodes (suddenly I care about performance, and don't even ask, it's a complex tradeoff of short and long term costs/benefits that'd take pages to explain) is a switch statement. No, in fact it's a series of "if"s. So the most frequent opcodes, those that can mess with the machine itself, get handled directly in the VM.

I like the technique of slicing answers into "cheap" and "nasty". Cheap is easy to change and changes often. Nasty is hard to change and changes rarely. Opcodes 240-254 are built-in opcodes; changing them requires modifying the VM source itself. These built-ins get to play with the instruction pointer, or "needle". That means the needle can be held in a register. This helps performance, at least theoretically.

Opcodes 0-239 are "atomics", and point to a look-up table of function addresses. As we register new atomics, each gets assigned a new number. The compiler uses that number (0-239) as opcode. These atomics are in their own source files. They get a VM context to talk to, although they cannot see or change the needle.

255 is the opcode for "do more complex stuff", which I'll now explain.

<A name="toc3-373" title="Extensibility" />
### Extensibility

Extensibility means people contributing. This should IMO be one of the first goals of any technically complex project: *how do I make it absurdly simple for people to give me their valuable time and knowledge?*

My goal here is to make it possible to add atomics in three places: inside the VM, very close to it, and outside it. Internal atomics are always going to be delicate, as they can easily break the VM. The 15-opcode address space is a Brutalist cage meant to stop random cruft getting in there.

The API for extensible atomics is as simple as I could make it. An atomic is a single C function, which receives a VM reference as argument, and returns 0 (silence is assent) or -1 (meaning "stop the machine!"). A function registers itself, it if wants to.

So for example the "check" atomic runs the ZeroScript self-tests. Here's how we probe the atomic:

    zs_vm_probe (self, s_check);

And here's the code for that function:

    static int
    s_check (zs_vm_t *self, zs_pipe_t *input, zs_pipe_t *output)
    {
        if (zs_vm_probing (self))
            zs_vm_register (self, "check", zs_type_nullary, "Run internal checks");
        else {
            int verbose = (zs_pipe_recv_whole (input) != 0);
            zs_lex_test (verbose);
            zs_pipe_test (verbose);
            zs_vm_test (verbose);
            zs_repl_test (verbose);
            zs_pipe_send_string (output, "Checks passed successfully");
        }
        return 0;
    }

For external atomics I want to add a "class" concept so that atomics are abstracted. The caller will register the class, which will register all its own atomics. This lets us add classes dynamically. The class will essentially be an opcode argument (255 + class + method).

<A name="toc3-406" title="Code Generation" />
### Code Generation

We use GSL code generation to build the core language pieces. There are two cases:

* Generating the scaling atomics. See [zs_scaling.gsl](https://github.com/LeptaSpace/zs/blob/master/src/zs_scaling.gsl) and [zs_units_si.xml](https://github.com/LeptaSpace/zs/blob/master/src/zs_units_si.xml), which produce the source code in [zs_units_si.h](https://github.com/LeptaSpace/zs/blob/master/src/zs_units_si.h).
* Generating the state machines. See zs_lex.xml and zs_repl.xml.

<A name="toc3-414" title="The Shell" />
### The Shell

The zs shell provides command history, editing, and tab completion using the Tab key or '?' (as I'm trying to not use special characters in the language, the question mark should remain free for the CLI) (We'll see). We use the editline library for this; it is a clone of the FSF readline function, though much smaller and without extra dependencies. One neat feature is that as you define commands, these become available in the shell.

To animate the state machines for the lexer and parser, run "zs -v".

<A name="toc3-421" title="Arguments" />
### Arguments

The nice thing about languages is the Internet Comments per Kiloline of Code (IC/KLOC) factor, easily 10-1,000 times higher than for things like protocols, security mechanisms, or library functions. Make a messy API and no-one gives a damn. Ah, but a language! Everyone has an opinion. I kind of like this, the long troll.

If you want to talk about minor details like my use of < and > for strings, be my guest. There is no real agenda here, except to provoke discussion, and to keep parsing as simple as possible for now. Asymmetric delimiters are trivial to parse. Curly ("real") quotes are too difficult to type. So < and > are a workable choice for now. It's also nice to be able to put ZeroScript strings inside C strings without any special escaping.

When doing an experiment, "everyone else does it this way, so you should too" is not valid science. Unless, the alternatives are known to be painful, toxic, or deadly. In fact doing stuff no-one expects is kind of exactly the point.

<A name="toc3-430" title="Other Goals" />
### Other Goals

Disclaimer: the "vision" thing is way overrated. I only add this section because it's fun.

This experiment started with the idea of a domain specific language for ZeroMQ examples. This is still a good idea. I'd like a language that compiles into clean C, Ruby, Python, whatever, using our code generation skills. It would solve a lot of problems in teaching ZeroMQ.

Imagine a ZeroScript runtime for embedded systems, so we can throw apps at 32KB devices. I really can't wait to try this. It changes the sense of "programmable devices". If you know Forth, you know it was used exactly for this kind of work (telescopes), long decades ago.

Imagine wrapping up libraries like Zyre, so ZeroScript apps can find each other on a network and throw themselves at each other. Start a Malamute broker with one command, then talk to it interactively and see what it says. Is the broker running in a thread, a process, or on a separate box?

Imagine ZeroMQ messaging that just looks like data pipes. The same semantics for a function as a process. And imagine the same patterns that we love from ZeroMQ used to connect functions: pub-sub, pipeline, request-reply. I don't know if this makes any sense at all, yet it feels crunchy.

Imagine a command shell that learns every time you use it, where you build up your working environment over weeks and years, which archives old unused functions and knows what to show you when you press Tab for auto-completion. Imagine being able to send such shells around to other people. "Here is my demo". It's not quite an application, and it's not quite a development environment. Someone's going to tell me Lisp already did it.

Since each box will have an arbitrary set of atomics, bytecode is not portable. However the compilation process is lossless, so that we can produce source code back from bytecode.

Perhaps the most compelling reason for a new language project is to give the ZeroMQ community an opportunity to work together. We are often fragmented across platforms and operating systems, yet we are solving the same kinds of problems over and over. A shared language would bring together valuable experience. This is the thing which excites me the most, which we managed to almost do using C (as it can be wrapped in anything, so ties together many cultural threads).

<A name="toc2-449" title="Design Notes" />
## Design Notes

* Any language aspect that takes more than 10 minutes to understand is too complex.
* Function names are case-sensitive because the real world is case sensitive (1 M vs. 1 m).
* Special characters are annoying and I want to reduce or eliminate the need on them. Some punctuation is OK.
* Real numbers and whole numbers are not the same set in reality. How much is 2 + 2? Anything from 3 to 5, if you are counting real things.

<A name="toc2-457" title="Bibliography" />
## Bibliography

* http://www.complang.tuwien.ac.at/forth/threaded-code.html
* http://en.wikipedia.org/wiki/Metric_prefix
* http://en.wikipedia.org/wiki/Arity
* http://en.wikipedia.org/wiki/Concatenative_programming_language
* https://wiki.haskell.org/Functional_programming

<A name="toc2-466" title="Technicalities" />
## Technicalities

<A name="toc3-469" title="Ownership and License" />
### Ownership and License

The contributors are listed in AUTHORS. This project uses the MPL v2 license, see LICENSE.

ZeroScript uses the [C4.1 (Collective Code Construction Contract)](http://rfc.zeromq.org/spec:22) process for contributions.

ZeroScript uses the [CLASS (C Language Style for Scalabilty)](http://rfc.zeromq.org/spec:21) guide for code style.

To report an issue, use the [ZeroScript issue tracker](https://github.com/lepaspace/zs/issues) at github.com.

<A name="toc3-480" title="Building and Installing" />
### Building and Installing

Here's how to build ZeroScript from GitHub:

    git clone git://github.com/zeromq/libzmq.git
    cd libzmq
    ./autogen.sh
    ./configure && make -j 4 check
    sudo make install
    sudo ldconfig
    cd ..

    git clone git://github.com/zeromq/czmq.git
    cd czmq
    ./autogen.sh
    ./configure && make -j 4 check
    sudo make install
    sudo ldconfig
    cd ..

    git clone git://github.com/leptaspace/zs.git
    cd zs
    ./autogen.sh
    ./configure && make -j 4 check
    sudo make install
    sudo ldconfig
    cd ..

You will need the pkg-config, libtool, and autoreconf packages.

Uses: https://github.com/troglobit/editline. Get the tarballs from http://troglobit.com/editline.html.

<A name="toc3-513" title="This Document" />
### This Document

This document is originally at README.txt and is built using [gitdown](http://github.com/imatix/gitdown).
