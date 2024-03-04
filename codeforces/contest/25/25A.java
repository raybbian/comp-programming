import java.util.*;
import java.io.*;

public class IQTest {
    static Kattio io = new Kattio();
    public static void main(String[] args) {
        int n = io.nextInt();
        int[] nums = new int[n + 1];
        int lastOdd, lastEven, oddCount, evenCount;
        lastOdd = lastEven = oddCount = evenCount = 0;
        for (int i = 1; i <= n; i++) {
            nums[i] = io.nextInt() % 2;
            if (nums[i] == 1) {
                lastOdd = i;
                oddCount++;
            }
            if (nums[i] == 0) {
                lastEven = i;
                evenCount++;
            }
        }

        if (oddCount == 1) io.println(lastOdd);
        if (evenCount == 1) io.println(lastEven);
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