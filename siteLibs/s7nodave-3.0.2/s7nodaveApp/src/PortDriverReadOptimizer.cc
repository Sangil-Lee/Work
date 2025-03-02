#include <cmath>
#include <map>
#include <tuple>

#include <string.h>

#include "PortDriverReadOptimizer.h"

namespace s7nodave {

using Optimizer = PortDriverReadOptimizer;
using ReadItem = Optimizer::ReadItem;
using ReadItemList = Optimizer::ReadItemList;

void Optimizer::optimizeWithinArea(ReadItemList& items, int maxPayloadSize) {
    // We need a sorted list later anyway. If we sort it now, the optimization
    // algorithm runs a bit faster.
    items.sort(ReadItem::pointerCompareStartByte);
    // Take the first item in the list and look for items that are
    // included in or including the item. If the item is not included
    // in another item, keep it.
    ReadItemList keptItems;
    while (!items.empty()) {
        auto firstItemIterator = items.begin();
        ReadItemPtr firstItem = *firstItemIterator;
        bool keepItem = true;
        // Start loop at second item
        auto i = ++(items.begin());
        while (i != items.end()) {
            ReadItemPtr item = *i;
            if (item->includesNoAreaCheck(*firstItem)) {
                // Add first item to found item and throw away first item
                // if add was successful.
                if (item->addReadItem(firstItem)) {
                    keepItem = false;
                }
                break;
            } else if (firstItem->includesNoAreaCheck(*item)) {
                // Add found item to first item and throw away found item
                if (firstItem->addReadItem(item)) {
                    // Return value of erase points to next element in list
                    i = items.erase(i);
                } else {
                    // Add was not successful for some reason, so we keep the
                    // item.
                    break;
                }
            } else if (firstItem->getEndByte() <= item->getStartByte()) {
                // The list is ordered by start byte (increasing). If the
                // selected item is already beyond the end of the first item,
                // we can stop looking.
                break;
            } else {
                // Proceed to next element
                i++;
            }
        }
        if (keepItem) {
            keptItems.push_back(firstItem);
        }
        items.erase(firstItemIterator);
    }
    // Move all items from keptItems to items.
    items.splice(items.end(), keptItems);

    // The list items now is a list of items, which are not included in any
    // of the other items. However, there may still be overlapping parts or two
    // items might at least be close to each other.
    // Now we build new items from these items by joining them together, if they
    // are separated by at most four bytes and the total item size is still
    // within the limits of the communication protocol.
    while (!items.empty()) {
        auto firstItemIterator = items.begin();
        ReadItemPtr firstItem = *firstItemIterator;
        // Start search with second item
        auto i = ++(items.begin());
        // Iterate until we reach the end of the list or the payload becomes too
        // large. We have to substract four bytes from the payload for the
        // header of the item.
        int startByte = firstItem->getStartByte();
        int endByte = firstItem->getEndByte();
        ReadItemList itemsInBlock;
        while (i != items.end()) {
            ReadItemPtr item = *i;
            // If adding the item would cause the maximum payload size to be
            // exceeded, we stop.
            if ((item->getEndByte() - startByte + 4) > maxPayloadSize) {
                break;
            }
            // We join the item if it is separated from the current block by at
            // most four bytes.
            if (item->getStartByte() <= endByte + 4) {
                endByte = item->getEndByte();
                itemsInBlock.push_back(item);
                // Erase item and increment iterator to next item
                i = items.erase(i);
            } else {
                // Next item is too far away. Close this block.
                break;
            }
        }
        if (itemsInBlock.empty()) {
            // There were no items to be added, so we just keep the existing
            // item.
            keptItems.push_back(firstItem);
            items.erase(firstItemIterator);
        } else {
            // Add the first item to the block and create a new item containing
            // all found items.
            itemsInBlock.push_front(firstItem);
            int itemLength = endByte - startByte;
            ReadItemPtr newItem(new ReadItem(firstItem->getArea(), firstItem->getAreaNumber(), startByte, itemLength));
            if (newItem->addReadItems(itemsInBlock)) {
                keptItems.push_back(newItem);
            } else {
                // Keep items
                keptItems.splice(keptItems.end(), itemsInBlock);
            }
            // Remove first item from list
            items.erase(firstItemIterator);
        }
    }

    // Finally, we place the optimized list in the original reference.
    items.splice(items.end(), keptItems);
}

ReadItemList Optimizer::createOptimizedList(asynS7nodaveReadItem *firstItem, int maxPayloadSize) {
    using AreaTuple = std::tuple<s7nodavePlcArea, int>;
    using AreaToItemsMap = std::map<AreaTuple, ReadItemList>;

    // Build map with all original requests in lists
    AreaToItemsMap areaToItems;
    asynS7nodaveReadItem *item = firstItem;
    while (item) {
        // Look for existing list for this area
        AreaTuple key = AreaTuple(item->area, item->areaNumber);
        auto entry = areaToItems.find(key);
        if (entry == areaToItems.end()) {
            // No list found, so create one.
            entry = areaToItems.insert(std::make_pair(key, ReadItemList())).first;
        }
        // Insert item into list
        ReadItemPtr newItem(new ReadItem(item));
        entry->second.push_back(newItem);
        item = item->next;
    }

    // Now optimize the list for each area and add it to the final list
    ReadItemList finalList;
    for (AreaToItemsMap::iterator i = areaToItems.begin(); i != areaToItems.end(); i++) {
        s7nodavePlcArea area = std::get<0>(i->first);
        ReadItemList areaList = i->second;
        // We can optimize the read requests for the input, output, data block,
        // and flag areas, so that overlapping or neighboring memory regions
        // are joined to a single request.
        // We do not optimize for the counter and timer areas, because those
        // areas have a different addressing scheme, that does not count in
        // bytes (but in words) and thus is not compatible with our optimization
        // code. Those areas are only supported for legacy applications anyway.
        if (area == plcAreaInputs || area == plcAreaOutputs || area == plcAreaDb || area == plcAreaFlags) {
            optimizeWithinArea(areaList, maxPayloadSize);
        }
        finalList.splice(finalList.end(), areaList);
    }
    return finalList;
}

void Optimizer::dispatchResults(ReadItemList& itemList) {
    auto it = itemList.begin();
    while (it != itemList.end()) {
        ReadItemPtr item = *it;
        item->dispatchReadResultToChildren();
        // Remove this element from list
        it = itemList.erase(it);
    }
}

} // namespace s7nodave
