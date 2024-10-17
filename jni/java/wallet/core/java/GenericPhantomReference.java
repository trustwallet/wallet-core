package wallet.core.java;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.util.HashSet;
import java.util.Set;

public class GenericPhantomReference extends PhantomReference<Object> {
    private final long handle;
    private final OnDeleteCallback onDelete;

    private static final Set<GenericPhantomReference> references = new HashSet<>();
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
        this.handle = handle;
        this.onDelete = onDelete;
    }

    public static void register(Object referent, long handle, OnDeleteCallback onDelete) {
        references.add(new GenericPhantomReference(referent, handle, onDelete));
    }

    private static void doDeletes() throws InterruptedException {
        while (true) {
            GenericPhantomReference ref = (GenericPhantomReference) queue.remove();
            ref.onDelete.onDelete(ref.handle);
            references.remove(ref);
        }
    }

    @FunctionalInterface
    public interface OnDeleteCallback {
        void onDelete(long handle);
    }
}