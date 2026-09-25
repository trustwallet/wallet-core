package com.trustwallet.core

import java.lang.ref.PhantomReference
import java.lang.ref.ReferenceQueue
import java.util.Collections

internal class GenericPhantomReference private constructor(
    referent: Any,
    private val handle: Long,
    private val onDelete: (Long) -> Unit,
) : PhantomReference<Any>(referent, queue) {

    companion object {
        private val references: MutableSet<GenericPhantomReference> = Collections.synchronizedSet(HashSet())
        private val queue: ReferenceQueue<Any> = ReferenceQueue()

        init {
            Thread {
                try {
                    doDeletes()
                } catch (e: InterruptedException) {
                    Thread.currentThread().interrupt()
                }
            }.apply {
                name = "WCFinalizingDaemon"
                isDaemon = true
                priority = Thread.NORM_PRIORITY
                start()
            }
        }

        fun register(
            referent: Any,
            handle: Long,
            onDelete: (Long) -> Unit,
        ) {
            references.add(GenericPhantomReference(referent, handle, onDelete))
        }

        private fun doDeletes() {
            while (true) {
                val ref = queue.remove() as GenericPhantomReference
                ref.onDelete(ref.handle)
                references.remove(ref)
            }
        }
    }
}
