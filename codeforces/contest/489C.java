import java.util.*;
import java.io.*;

public class GivenLengthAndSumOfDigits {
    static Kattio io = new Kattio();
    public static void main(String[] args) {
        int m = io.nextInt();
        int s = io.nextInt();
        boolean can;
        if ((s == 0 && m != 1) || s > m * 9) can = false;
        else can = true;

        int[] minAns = new int[m + 1];
        int temp = s - 1;
        for (int i = m; i >= 1; i--) {
            if (temp >= 9) {
                temp -= 9;
                minAns[i] = 9;
            } else {
                minAns[i] = temp;
                temp = 0;
            }
        }
        minAns[1]++;

        int[] maxAns = new int[m + 1];
        for (int i = 1; i <= m; i++) {
            if (s >= 9) {
                s -= 9;
                maxAns[i] = 9;
            } else {
                maxAns[i] = s;
                s = 0;
            }
        }

        if (!can) {
            io.println("-1 -1");
        } else {
            for (int i = 1; i <= m; i++) {
                io.print(minAns[i]);
            }
            io.print(" ");
            for (int i = 1; i <= m; i++) {
                io.print(maxAns[i]);
            }
            io.println();
        }
        
        io.close();
    }
    static class Kattio extends PrintWriter {
        private BufferedReader r;
        private StringTokenizer st;

        public Kattio() { this(System.in, System.out); }
        public Kattio(InputStream i, OutputStream o) {
            super(o);
            r = new BufferedReader(new InputStreamReader(i));
        }
        public Kattio(String problemName) throws IOException {
            super(new FileWriter(problemName + ".out"));
            r = new BufferedReader(new FileReader(problemName + ".in"));
        }
        public String next() {
            try {
                while (st == null || !st.hasMoreTokens())
                    st = new StringTokenizer(r.readLine());
                return st.nextToken();
            } catch (Exception e) { }
            return null;
        }
        public int nextInt() { return Integer.parseInt(next()); }
        public double nextDouble() { return Double.parseDouble(next()); }
        public long nextLong() { return Long.parseLong(next()); }
    }
}