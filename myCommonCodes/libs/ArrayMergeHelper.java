import java.util.*;
import java.util.function.Function;

public class ArrayMergeHelper<T> {
     /** both iterators should be already sorted.
     * 
     * @param compare Function to compare two array object.
     *                Return true if first is prior to second one.
     * */
     public List<T> mergeOpItrs(Iterator<T> first, Iterator<T> second,
                             Function<Map.Entry<T, T>, Boolean> compare) {
        if (!first.hasNext() && ! second.hasNext()) {
            return null;
        }
        ArrayList<T> result = new ArrayList<>();
        T item1 = first.next();
        T item2 = second.next();

        while (true) {
            if (item1 == null && item2 == null) {
                break;
            }
            if (item1 == null) {
                result.add(item2);
                second.forEachRemaining(result::add);
                break;
            }
            if (item2 == null) {
                result.add(item1);
                first.forEachRemaining(result::add);
                break;
            }
            if (compare.apply(new AbstractMap.SimpleEntry<T, T>(item1, item2))) {
                result.add(item1);
                item1 = first.next();
            }
            else {
                result.add(item2);
                item2 = second.next();
            }
        }
        return result;
    }
    
}
