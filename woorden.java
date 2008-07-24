import java.io.*;

public class woorden
{
    /** A standard-compliant Brainfuck interpreter.

    The register space is unbounded on one side.
    Read semantics are: on end-of-file, leave target register unchanged.

    If an IndexOutOfBoundsException is thrown, this means either the
    braces in the source are unbalanced, or the program tried to access
    a cell before the beginning of the tape. If the program exits
    succesfully, the function just returns.

    @param s    The Brainfuck program source code; all characters not in the
                eight-symbol Brainfuck alphabet are ignored.
    @param in   The input source
    @param out  The output sink

    @throws IOException                 if an I/O operation fails
    @throws IndexOutOfBoundsException   on failure
    */
    static void brainfuck(String s, InputStream in, OutputStream out) throws IOException
    {
        int[] o = new int[s.length()];
        for (int t, p = -1, n = 0; n < o.length; ++n)
            switch(s.charAt(n)) {
            case '[': o[n] = p; p = n; break;
            case ']': t = o[p]; o[n] = p; o[p] = n; p = t; break;
            }
        byte[] r = new byte[8];
        for (int c, d = 0, i = 0; i < o.length; ++i)
            switch(s.charAt(i))
            {
            case '>': if (++d == r.length) System.arraycopy(
                r, 0, r = new byte[2*r.length], 0, d ); break;
            case '<': --d; break;
            case '+': ++r[d]; break;
            case '-': --r[d]; break;
            case '[': if (r[d] == 0) i = o[i]; break;
            case ']': if (r[d] != 0) i = o[i]; break;
            case ',': if ((c = in.read()) >= 0) r[d] = (byte)c; break;
            case '.': out.write(r[d]); out.flush(); break;
            }
    }

    public static void main(String[] args) throws IOException
    {
        /* Asks for a line of input, then splits the input into words and
           displays each word on numbered lines. Program ends when the user
           types a sentence starting with the word "end". */
        brainfuck(
"+>+<[>[->[-]>[-]+<++++++++[->+++++++++<]>.+<++++++[->++++++<]>.++.+++++.-.[-<+>]"+
"<[-->+<]>.-<+++++[->-----<]>.[-]<<]>>>>,[->+>+<<]>>[-<<+>>]<<<+>>----------[<<->"+
">[-]]<<[-<<<+>>>>++++++++++++++++++++++<]>[->+>+<<]>>[-<<+>>]<<<+>>>++++[-<-----"+
"--->]<[<<->>[-]]<<[-<[->>>+<<<]>>>[-<<+<+>>>]>>[-<+<+>>]<[->+<]<->++++++++++[-<-"+
"--------->]<[[-]<<+>>]>>>[-<<+<+>>>]<<[->>+<<]++++++++++[-<----------->]<[[-]<<+"+
">>]>>>>[-<<<+<+>>>>]<<<[->>>+<<<]++++++++++[-<---------->]<[[-]<<+>>]>>>>>[-<<<<"+
"+<+>>>>>]<<<<[->>>>+<<<<]<[[-]<<+>>]+<<[[-]>>-<<]>>[-<<<<<<->>>>>>>>[-]<<]>>[-<+"+
"<+>>]<[->+<]<[[-]<<<+[->+>>+<<<]>[-<+>]+>>----------[[-]<<->>]<<[[-]<----------<"+
"+>>]<<[->>+>>+<<<<]>>[-<<+>>]>>[>++++++++[-<++++++>]<.[-]]<<<[->+>>+<<<]>[-<+>]>"+
">>++++++++[-<++++++>]<.[-]<+<+++++[->+++++<]>.-<+++++[->-----<]>.>>>[.>]<[[-]<]<"+
"++++++++++.[-]]<<]>[->+>+<<]>>[-<<+>>]<-->+++++[-<------>]<[[-]<<+>>]<<[->[->+>+"+
"<<]>>[-<<+>>]<->>[>]+[<]<[->>[>]<+[<]<]<<]<<<<]", System.in, System.out);
    }
}
