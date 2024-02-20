import java.util.*;
import java.io.*;

public class HW8P3RB {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int m = sc.nextInt();
        int[] connectedCount = new int[n];
        for (int i = 0; i < m; i++) {
            connectedCount[sc.nextInt() - 1]++;
            connectedCount[sc.nextInt() - 1]++;
        }

        int oneCount = 0;
        int twoCount = 0;
        int nCount = 0;
        for (int i = 0; i < n; i++) {
            if (connectedCount[i] == 1) oneCount++;
            if (connectedCount[i] == 2) twoCount++;
            if (connectedCount[i] == m) nCount++;
        }
        if (nCount == 1 && oneCount == n - 1 && twoCount == 0) System.out.println("star topology");
        else if (twoCount == n - 2 && oneCount == 2 && nCount == 0) System.out.println("bus topology");
        else if (twoCount == n && oneCount == 0 && nCount == 0) System.out.println("ring topology");
        else System.out.println("unknown topology");
    }
}