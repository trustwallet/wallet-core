package wallet.core.java;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.util.Set;
import java.util.HashSet;
import java.util.Collections;

public class GenericPhantomReference extends PhantomReference<Object> {
    private final long nativeHandle;
    private final OnDeleteCallback onDeleteCallback;

    private static final Set<GenericPhantomReference> references = Collections.synchronizedSet(new HashSet<>());
    private static final ReferenceQueue<Object> queue = new ReferenceQueue<>();

    static {
        Thread finalizingDaemon = new Thread(() -> {
            try {
                doDeletes();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        finalizingDaemon.setName("WCFinalizingDaemon");
        finalizingDaemon.setDaemon(true);
        finalizingDaemon.setPriority(Thread.NORM_PRIORITY);
        finalizingDaemon.start();
    }

    private GenericPhantomReference(Object referent, long handle, OnDeleteCallback onDelete) {
        super(referent, queue);
        this.nativeHandle = handle;
        this.onDeleteCallback = onDelete;
    }

    public static void register(Object referent, long handle, OnDeleteCallback onDelete) {
        references.add(new GenericPhantomReference(referent, handle, onDelete));
    }

    private static void doDeletes() throws InterruptedException {
        GenericPhantomReference ref = (GenericPhantomReference) queue.remove();
        for (; ref != null; ref = (GenericPhantomReference) queue.remove()) {
            ref.onDeleteCallback.nativeDelete(ref.nativeHandle);
            references.remove(ref);
        }
    }

    @FunctionalInterface
    public interface OnDeleteCallback {
        void nativeDelete(long handle);
    }
}
