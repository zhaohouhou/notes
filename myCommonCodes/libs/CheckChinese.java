/**
Java String 采用 UTF-16 编码方式存储所有字符。string之间不存在编码变换问题。
只有 字符到字节 或者 字节到字符 的转换才存在编码转码,
编码转换场景主要在I/O，如文件、屏幕、数据库、浏览器、服务器I/O。

以下工具类判断java字符串中是否有/为中文
*/

public class CheckChinese {
public static void main(String[] args) {
// 纯英文
        String s1 = "Hello,Tom.!@#$%^&*()_+-={}|[];':\"?";
// 纯中文（不含中文标点）
        String s2 = "你好中国";
// 纯中文（含中文标点）
        String s3 = "你好，中国。《》：“”‘'；（）【】！￥、";
// 韩文
        String s4 = "한국어난";
// 日文
        String s5 = "ぎじゅつ";
// 特殊字符
        String s6 = "��";
        System.out.println("s1是否包含中文：" + hasChineseByRange(s1));// false
        System.out.println("s2是否包含中文：" + hasChineseByRange(s2));// true
        System.out.println("-------分割线-------");
        System.out.println("s1是否全是中文：" + isChineseByRange(s1));// false
        System.out.println("s2是否全是中文：" + isChineseByRange(s2));// true
}
/**
 * 是否包含中文字符<br>
 * 包含中文标点符号<br>
 *
 * @param str
 * @return
 */
public static boolean hasChinese(String str) {
        if (str == null) {
                return false;
        }
        char[] ch = str.toCharArray();
        for (char c : ch) {
                if (isChinese(c)) {
                        return true;
                }
        }
        return false;
}
/**
 * 是否全是中文字符<br>
 * 包含中文标点符号<br>
 *
 * @param str
 * @return
 */
public static boolean isChinese(String str) {
        if (str == null) {
                return false;
        }
        char[] ch = str.toCharArray();
        for (char c : ch) {
                if (!isChinese(c)) {
                        return false;
                }
        }
        return true;
}
/**
 * 是否是中文字符<br>
 * 包含中文标点符号<br>
 *
 * @param c
 * @return
 */
private static boolean isChinese(char c) {
        Character.UnicodeBlock ub = Character.UnicodeBlock.of(c);
        if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C) {
                return true;
        } else if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D) {
                return true;
        } else if (ub == Character.UnicodeBlock.GENERAL_PUNCTUATION) {
                return true;
        } else if (ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
                return true;
        }
        return false;
}
/**
 * 是否包含汉字<br>
 * 根据汉字编码范围进行判断<br>
 * CJK统一汉字（不包含中文的，。《》（）“‘'”、！￥等符号）<br>
 *
 * @param str
 * @return
 */
public static boolean hasChineseByReg(String str) {
        if (str == null) {
                return false;
        }
        Pattern pattern = Pattern.compile("[\\u4E00-\\u9FBF]+");
        return pattern.matcher(str).find();
}
/**
 * 是否全是汉字<br>
 * 根据汉字编码范围进行判断<br>
 * CJK统一汉字（不包含中文的，。《》（）“‘'”、！￥等符号）<br>
 *
 * @param str
 * @return
 */
public static boolean isChineseByReg(String str) {
        if (str == null) {
                return false;
        }
        Pattern pattern = Pattern.compile("[\\u4E00-\\u9FBF]+");
        return pattern.matcher(str).matches();
}
/**
 * 是否包含汉字<br>
 * 根据汉字编码范围进行判断<br>
 * CJK统一汉字（不包含中文的，。《》（）“‘'”、！￥等符号）<br>
 *
 * @param str
 * @return
 */
public static boolean hasChineseByRange(String str) {
        if (str == null) {
                return false;
        }
        char[] ch = str.toCharArray();
        for (char c : ch) {
                if (c >= 0x4E00 && c <= 0x9FBF) {
                        return true;
                }
        }
        return false;
}
/**
 * 是否全是汉字<br>
 * 根据汉字编码范围进行判断<br>
 * CJK统一汉字（不包含中文的，。《》（）“‘'”、！￥等符号）<br>
 *
 * @param str
 * @return
 */
public static boolean isChineseByRange(String str) {
        if (str == null) {
                return false;
        }
        char[] ch = str.toCharArray();
        for (char c : ch) {
                if (c < 0x4E00 || c > 0x9FBF) {
                        return false;
                }
        }
        return true;
}
}
