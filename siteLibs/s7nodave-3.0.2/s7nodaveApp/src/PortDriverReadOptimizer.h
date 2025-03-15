#ifndef s7nodave_PortDriverReadOptimizer_h
#define s7nodave_PortDriverReadOptimizer_h

#include <list>

#include "PortDriverReadItem.h"

namespace s7nodave {

/**
 * Optimized a set of read items. This optimization is performed by aggregating
 * read items that refer to the same or close-by memory ranges into a single
 * item, thus reducing the number of items and avoiding memory ranges to be
 * transferred twice.
 * This class is intended for use by S7nodavePortDriver only.
 */
class PortDriverReadOptimizer {
public:
    using ReadItem = PortDriverReadItem;
    using ReadItemPtr = std::shared_ptr<ReadItem>;
    using ReadItemList = std::list<ReadItemPtr>;

    /**
     * Creates an optimized list of read items from the passed chained read item
     * structure. The maximum payload size passed is the maximum payload size
     * PDU size without the 14 byte command header. For each item requested,
     * another 4 bytes have to be allocated for the item header.
     */
    static ReadItemList createOptimizedList(asynS7nodaveReadItem *firstItem, int maxPayloadSize);

    /**
     * Calls the dispatchReadResultsToChildren of each read item in the list
     * and subsequently removes the item from the list.
     */
    static void dispatchResults(ReadItemList& readItemList);
private:
    /**
     * Private constructor. The public methods of this class are all static, so
     * there is no need to create an instance.
     */
    PortDriverReadOptimizer() {
    };

    /**
     * Called by createOptimizedList to optimize the number of items within
     * a single memory area.
     */
    static void optimizeWithinArea(ReadItemList& readItemList, int maxPayloadSize);
};

} // namespace s7nodave

#endif // s7nodave_PortDriverReadOptimizer_h
