<h1 align="center">Interpreter-in-C</h1>
<h3 align="center">Interpreter in C with custom syntax and the ability to debug programs</h3>
<h3 align="center">Task in details: №10 in task.pdf</h3>

<ul>
    <li>"#" - one-line comment, all subsequent text in the line will not be processed;</li>
    <li>Multi-line comment enclosed between "[" and "]";</li>
    <li>The variable name can consist of Latin letters, digits and the symbol "_". Name can't start with a digit. Uppercase and lowecase letters are not equal;</li>
    <li>Variable type is uint32_t;</li>
    <li>In the settings file user can add synonyms to the commands. Example for assign command: "= add", where "=" is original command;</li>
    <li>User can add a setting for the location of operation relative to the arguments. More details in "task.pdf";</li>
    <li>Base assign, base input, base output are arguments of command line. If one of them is not indicated, it will be automatically equated to 10. Base is in the range [2; 36];</li>
    <li>";" - instruction separator;</li>
    <li>The program file is specified as a command line argument;</li>
</ul>
<hr>
<h3 align="center">Some additions to the task</h3>
<p>Command line arguments. Program will not work without arguments marked with "*"</p>
<ol>
    <li>File with a program*</li>
    <li>File with settings</li>
    <li>Base assign</li>
    <li>Base input</li>
    <li>Base output</li>
</ol>
<p>Current setting are in the "config.txt" file. Program will not work without it.</p>

<h3 align="center">How it works?</h3>
<p>Interpreter open the "config.txt" file and search for custom settings. If first line is "STANDART", searching will be stopped. Then program checks, if there is a settings file in command line argument: if so, it change some instructions with newest. After this program fills out the bases (align, input and output) if needed and starts interpreteting program file</p>

<h3>Settings file example:</h3>
<code>
    right= #comment<br>
    (op)<br>
    add sum<br>
    #mult prod and this is also comment<br>
    [sub minus<br>
    pow ^ and this..]<br>
    div /<br>
    rem %<br>
    xor <><br>
    xor ><<br>
    input in<br>
    output print<br>
    = -><br>
</code>

<h3>Program example <small>(assign base = 16)</small>:</h3>
<code>
    var_1 = 1F4;<br>
    var_2 = mult(var_1,4);<br>
    Var3 = add(div(var_2,5), rem(var_1,2));<br>
    print(Var3);
</code>