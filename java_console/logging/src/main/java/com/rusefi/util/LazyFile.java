package com.rusefi.util;

import java.io.*;
import java.nio.file.Paths;
import java.util.Scanner;
import java.util.regex.Pattern;

/**
 * This file would override file content only of content has changed, disregarding the magic tag line.
 */
public class LazyFile implements Output {
    public static String TEST = "test_file_name";

    private Writer fw = null;

    public LazyFile(String filename) {
        if (!TEST.equals(filename)) {
            try {
                fw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(filename), IoUtils.CHARSET));
            } catch (IOException e) {
                // ignore?
            }
        }
    }

    @Override
    public void write(String line) {
        if (fw != null) {
            try {
                fw.write(line);
            } catch (IOException e) {
            }
        }
    }

    @Override
    public void close() throws IOException {
        if (fw != null) {
            fw.close();
        }
    }
}
