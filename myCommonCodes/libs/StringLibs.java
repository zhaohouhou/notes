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

    /**
     * Generate a random(temporarily unique) string.
     * Characters: {0-9, a-Z}
     * */
    public static String randStr(){
        String characters = "0123456789" +
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ" +
                          "abcdefghijklmnopqrstuvwxyz";
        int length = 15;
        StringBuilder str = new StringBuilder();
        for (int i = 0; i <length ; i++)
        {
          //System.currentTimeMillis() is not enough 'random'!
            Random rdm = new Random(System.nanoTime());
            int index = Math.abs(rdm.nextInt())%(characters.length());
            str.append(characters.charAt(index));
        }
        return str.toString();
    }
}
