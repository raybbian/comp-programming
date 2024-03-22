import java.util.*;
import java.io.*;

public class Boredom {
    public static void main(String[] args) {
        Kattio io = new Kattio();
        int n = io.nextInt();
        int[] sequence = new int[100001];
        int[] count = new int[100001];
        int maxA = 0;
        for (int i = 1; i <= n; i++) {
            sequence[i] = io.nextInt();
            count[sequence[i]]++;
            maxA = Math.max(sequence[i], maxA);
        }

        long[] dp = new long[maxA + 1];
        dp[1] = count[1];
        dp[0] = 0;
        for (int i = 2; i <= maxA; i++) { //keep  + the amount of points from this
            dp[i] = Math.max(dp[i - 1], dp[i - 2] + (long) count[i] * i);
        }
        io.println(dp[maxA]);
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