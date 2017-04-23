public class StringLibs
{
  /**
    * Split string by s.
    * */
  public static String[] split(String s, String string)
      {
          ArrayList<String> result = new ArrayList<>();
          int start, end;
          while (string.contains(s)){
              start = 0;
              end = string.indexOf(s);
              result.add(string.substring(start, end));
              string = string.substring(end + s.length());
          }
          result.add(string);
          return result.toArray(new String[result.size()]);
      }

      /**
     * Check if String 'pre' is a package-prefix of string 'test'.
     *
     * e.g:
     * isPrefix("com","com") => true;
     * isPrefix("com","come") => false;
     * isPrefix("com","com.abc") => true;
     * isPrefix("com.abc","com") => false;
     * isPrefix("com.abc","com.abcd") => false;
     * */
    public static boolean isPrefix(String pre, String test)
    {
        if(! test.startsWith(pre))
            return false;
        String[] preList = pre.split("\\.");
        String[] testList = test.split("\\.");
        for (int i = 0; i < preList.length; i++ )
        {
            if(! preList[i].equals(testList[i]))
                return false;
        }
        return true;
    }
}
